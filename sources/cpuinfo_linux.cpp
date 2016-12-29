/**
 *  Provides functions for retrieving Cpu information on a linux system.
 *  The constructor opens the /proc/stats and /proc/cpuinfo files for
 *  reading cpu information. If that fails an exception is thrown. Otherwise
 *  we fill up the cpu field buffers then exit.
 */


#include "cpuinfo.h"

// some common vendor names
std::vector<std::string>
CpuInformation::m_vendorNames = {"AMD","Intel","VMWare"};

// the following functions read info from /proc/cpuinfo
int readCpuStats (FILE* fp, uint64_t* fields);
bool readCpuStatsLine(FILE* fp, int lineNumber ,uint64_t* fields);


/**
 * @brief CpuInformation constructor
 *
 * Opens the /proc/stats and /proc/cpuinfo files for
 * cpu information. If that fails an exception is thrown. Otherwise
 * we fill up the cpu field buffers then exit.
 */
CpuInformation::CpuInformation()
{
    m_logger = spdlog::get("cysboardLogger");

    // Grab cpu info from /proc/cpuinfo
    m_fpInfo = fopen("/proc/cpuinfo", "r");

    if(m_fpInfo == nullptr)
        throw std::runtime_error( std::string(__FUNCTION__) + "Failed to open /proc/cpuinfo");

    // Read in cpu timming data from /proc/stat
    m_fpStats = fopen("/proc/stat", "r");

    if(m_fpStats == nullptr)
        throw std::runtime_error(std::string(__FUNCTION__) + "Failed to open /proc/stats");

    // capture the first tick values
    int i, cpus = 0;
    while (readCpuStats(m_fpStats, (uint64_t*)m_ticks.fields) != -1){
        for (i = 0, m_ticks.totalTick[cpus] = 0; i < 10; i++) {
            m_ticks.totalTick[cpus] += m_ticks.fields[i];
        }

        // third field is the amount of time spent doing nothing
        m_ticks.idle[cpus] = m_ticks.fields[3];

        cpus++;
    }

    // subtract 1 for number of cores
    m_numOfCores = cpus - 1;
}


/**
 * @brief CpuInformation Destructor
 */
CpuInformation::~CpuInformation() {
    fclose(m_fpStats);
    fclose(m_fpInfo);
}


/**
 * @brief Gets the cpu's name.
 *
 * Retrieves the cpu name returned from the cpuid instruction.
 *
 * @return The cpu brand name as an std::string
 */
std::string CpuInformation::getName()
{
    std::string brandName;

    for(long long i = m_cpuid.PROCESSOR_BRAND;
                  i <= m_cpuid.PROCESSOR_BRAND + 2; i++)
    {
        m_cpuid.execute(i);

        brandName += std::string((const char*)&m_cpuid.getEAX(), 4);
        brandName += std::string((const char*)&m_cpuid.getEBX(), 4);
        brandName += std::string((const char*)&m_cpuid.getECX(), 4);
        brandName += std::string((const char*)&m_cpuid.getEDX(), 4);
    }

    return brandName;
}


/**
 * @brief Gets the cpu vendor's name
 *
 * Retrieves the cpu vendor's name returned by the cpuid instruction.
 *
 * @return The vendor of the cpu as an std::string
 */
std::string CpuInformation::getVendor()
{
    std::string parsedVendorName;

    m_cpuid.execute(m_cpuid.PROCESSOR_VENDOR);
    parsedVendorName += std::string((const char*)&m_cpuid.getEBX(), 4);
    parsedVendorName += std::string((const char*)&m_cpuid.getEDX(), 4);
    parsedVendorName += std::string((const char*)&m_cpuid.getECX(), 4);

    // check if the name is in our list of names
    for(auto name : m_vendorNames) {
        if(parsedVendorName.find(name) != std::string::npos) {
            parsedVendorName = name;
        }
    }

    return parsedVendorName;
}


/**
 * @brief Gets the number of physical cpus on the system
 *
 * Retrieves the number of **physical** cpus on the system
 * by calling sysconf(_SC_NPROCESSORS_ONLN). On linux this returns
 * the number of online processors, which is almost always the number of cores
 * on a normal desktop system.
 *
 * @return The number of physical cpus on the system
 */
int CpuInformation::getNumberOfCores() {
    return m_numOfCores;
}


/**
 * @brief Gets the cpu architecture
 *
 * Uses the utsname struct to retrieve the machine name
 *
 * @return the architecture on the cpu as an std::string
 */
std::string CpuInformation::getArchitecture() {
    utsname unameStruct;
    uname(&unameStruct);

    return std::string(unameStruct.machine);
}


/**
 * @brief Gets the speed of the cpu (all cores)
 *
 * Retrieves the speed of the cpu using data in /proc/cpuinfo
 *
 * @return The cpu speed as a double
 */
double CpuInformation::getCurrentSpeed(int core)
{
    double speed;
    core -= 1;

    std::stringstream ss;
    ss << readCpuInfoField(core, CPUINFO_SPEED);
    ss >> speed;

    return speed;
}


/**
 * @brief Retrieves a field value from /proc/cpuinfo
 * @param core  Which core the field is relative to
 * @param fieldName Which field to read
 * @return The field value as an std::string
 */
std::string CpuInformation::readCpuInfoField(int core, std::string fieldName)
{
    int coreCount;
    char line[30], *ptrMatch, separator;

    std::stringstream strStream;
    std::string value, processor;

    while(fgets(line, sizeof(line), m_fpInfo) != NULL) {
        if((ptrMatch = strstr(line, CPUINFO_PROCESSOR)) != NULL) {
            // clear all flags set by the stringstream
            strStream.clear();
            // clear data
            strStream.str("");
            strStream << ptrMatch;
            // parse this line: processor : [core number]
            // where [core number] is {0,1,2,3,...}
            strStream >> processor >> separator >> coreCount;

            // if the specified core is found
            if(coreCount == core) {
                // continue reading the file until we fiind "cpu MHz" field
                while(fgets(line, sizeof(line), m_fpInfo) != NULL) {
                    if((ptrMatch = strstr(line, fieldName.c_str())) != NULL) {
                        std::string matchedString(ptrMatch);
                        // find the first value of the seperator
                        size_t endPos = matchedString.find_first_of(":");
                        // add 2 to endPos because there is space between
                        // fieldName and value
                        value = matchedString.substr(endPos+2);

                        break;
                    }
                }

                break;
            }
        }
    }

    // reset the file pointer for the next call
    fseek(m_fpInfo, 0, SEEK_SET);
    fflush(m_fpInfo);

    return value;
}


/**
 * @brief Gets the usage percentage of a cpu core
 * @param core The core number (1, 2 etc). zero is for the total
 * @return The cpu core usage as a double
 */
double CpuInformation::getCoreUsagePercentage(int core)
{
    // if the core parameter is not greater than -1 abort.
    // used -1 so we can pass 0 for the total percentage usage
    assert(core > -1);

    usleep(100000);

    fseek(m_fpStats, 0, SEEK_SET);
    fflush(m_fpStats);

    // assign previous ticks to old ones to calculate change in ticks
    m_ticks.totalTickOld[core] = m_ticks.totalTick[core];
    m_ticks.idleOld[core] = m_ticks.idle[core];

    // read a line with the tick values for this core
    if(!readCpuStatsLine(m_fpStats, core, (uint64_t*)m_ticks.fields)) {
        return 0;
    }

    // find the total and idle values
    m_ticks.totalTick[core] = 0;

    for(int i = 0; i < 10; i++) {
        m_ticks.totalTick[core] += m_ticks.fields[i];
    }

    m_ticks.idle[core] = m_ticks.fields[3];

    // calculate the change in the values
    m_ticks.delTotalTick[core] = m_ticks.totalTick[core] - m_ticks.totalTickOld[core];
    m_ticks.delIdle[core] = m_ticks.idle[core] - m_ticks.idleOld[core];

    // calculate percentage
    double percentage = ((m_ticks.delTotalTick[core] - m_ticks.delIdle[core]) /
                                  (double)m_ticks.delTotalTick[core]) * 100;

    return percentage;
}


/**
 * @brief Gets the total usage percentage of the cpu
 *
 * Passing 0 to CpuInformation::getCoreUsagePercentage() will give
 * the total usage percentage
 *
 * @return
 */
double CpuInformation::getTotalUsagePercentage() {
   return this->getCoreUsagePercentage(0);
}


/**
 * @brief CpuInformation::getCacheSize
 * @param core Which core the cache size belongs to
 * @return The cache size as a double
 */
double CpuInformation::getCacheSize(int core) {
    double size;
    core -= 1;

    std::stringstream ss;
    ss << readCpuInfoField(core, CPUINFO_CACHE_SIZE);
    ss >> size;

    return size;
}


/**
 * @brief readCpuStats
 * @param fp
 * @param fields
 * @return
 */
int readCpuStats (FILE *fp, uint64_t *fields)
{
    int retval;
    char buffer[BUF_SIZE];

    if (!fgets (buffer, BUF_SIZE, fp)){
        perror ("Error");
    }

    retval = sscanf (buffer, "c%*s %Lu %Lu %Lu %Lu %Lu %Lu %Lu"\
                             " %Lu %Lu %Lu",
                                &fields[0],
                                &fields[1],
                                &fields[2],
                                &fields[3],
                                &fields[4],
                                &fields[5],
                                &fields[6],
                                &fields[7],
                                &fields[8],
                                &fields[9]);

    // at least four fields must be read
    if (retval == 0) {
        return -1;
    }

    if (retval < 4) {
        std::cerr << "Failed to read /proc/stats cpu field\n";
        return 0;
    }

    return 1;
}


/**
 * @brief Reads a single line from /proc/cpuinfo
 * @param fp A prepared file pointer to /proc/cpuinfo
 * @param lineNumber Which line number to read
 * @param fields An array to hold the fields
 * @return
 */
bool readCpuStatsLine(FILE* fp, int lineNumber, uint64_t* fields)
{
    char buffer[BUF_SIZE];

    int linesRead = 0;
    while(linesRead <= lineNumber) {
        if (!fgets (buffer, BUF_SIZE, fp)) {
            perror ("Error. Failed to read line from /proc/stat");
        }

        linesRead++;
    }

    const char* formatString = "c%*s %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu";
    int retVal = sscanf (buffer, formatString, &fields[0], &fields[1],
                                               &fields[2], &fields[3],
                                               &fields[4], &fields[5],
                                               &fields[6], &fields[7],
                                               &fields[8], &fields[9]);

    // if the number of values read are less than 4 or 0
    if(retVal == 0 || retVal < 4) {
        return false;
    }

    return true;
}


