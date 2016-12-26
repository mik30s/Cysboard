/**
 * @brief Initializes a cpuinfo object
 *
 * Fills the m_systeminfo private attribute for use.
 * Sets the pdhcounter paths for processor information
 * It will throw an exception when opening a pdh query fails
 */


#include "cpuinfo.h"


const std::vector<std::string> CpuInformation::m_vendorNames = {"AMD","Intel","VMWare"};


CpuInformation::CpuInformation(){
    PDH_STATUS status;

    GetSystemInfo(&m_systemInfo);
    // number of pdh counters is number of processors + 1, for the total cpu percentage
    m_numberOfPdhCounters = m_systemInfo.dwNumberOfProcessors + 1;

    DWORD pathSize = m_numberOfPdhCounters-1;
    LPTSTR path = (LPTSTR)GlobalAlloc(GPTR, pathSize);
    status = PdhExpandWildCardPath(NULL, L"\\Processor(*/*#*)\\%% Processor Time", path, &pathSize, NULL);
    if(status == PDH_MORE_DATA){
        pathSize++;
        GlobalFree(path);
        path = (LPTSTR)GlobalAlloc(GPTR, pathSize);

        status = PdhExpandWildCardPath(NULL, L"\\Processor(*/*#*)\\%% Processor Time", path, &pathSize, NULL);
        if(status == PDH_CSTATUS_VALID_DATA){

        }
    }

    // collect first sample
    status = PdhCollectQueryData(m_pdhQuery);
    if(status != ERROR_SUCCESS){
        m_logger->critical("Pdh Error {0:x}",status);
        throw BadClassException(__FUNCTION__, "PdhCollectQueryData() Failed");
    }
}


/**
 * @brief Destroys a CpuInformation Object
 */
CpuInformation::~CpuInformation(){
}


/**
 * @brief Gets the cpu name.
 *
 * Retrieves the cpu name returned from the cpuid instruction.
 * cpuid was used for because of its ease of use.
 * Instead of having to parse the information from /proc/cpuinfo on linux
 *
 * @return The cpu brand name as an std::string
 */
std::string CpuInformation::getName(){
    std::string brandName;

    for(unsigned i = m_cpuid.PROCESSOR_BRAND;
                 i <= m_cpuid.PROCESSOR_BRAND+2; i++)
    {
        this->m_cpuid.execute(i);
        brandName += std::string((const char*)&m_cpuid.getEAX(), 4);
        brandName += std::string((const char*)&m_cpuid.getEBX(), 4);
        brandName += std::string((const char*)&m_cpuid.getECX(), 4);
        brandName += std::string((const char*)&m_cpuid.getEDX(), 4);
    }

    return brandName;

//    return retVal;
}


/**
 * @brief Gets the cpu vendor's name
 *
 * Retrieves the cpu vendor's name returned by the cpuid instruction.
 * Cpuid was prefered because it is easier to use.
 * Instead of having to parse the information from /proc/cpuinfo on linux
 *
 * @return The vendor of the cpu as an std::string
 */
std::string CpuInformation::getVendor(){
    std::string parsedVendorName;

    m_cpuid.execute(m_cpuid.PROCESSOR_VENDOR);
    parsedVendorName += std::string((const char*)&m_cpuid.getEBX(), 4);
    parsedVendorName += std::string((const char*)&m_cpuid.getEDX(), 4);
    parsedVendorName += std::string((const char*)&m_cpuid.getECX(), 4);

    // check if the name is in our list
    for(auto name : m_vendorNames){
        if(parsedVendorName.find(name) != std::string::npos)
            parsedVendorName = name;
    }

    return parsedVendorName;
}


/**
 * @brief Gets the number of physical cpus on the system
 *
 * Retrieves the number of **physical** cpus on the system
 *
 * @return The number of physical cpus on the system
 */
int CpuInformation::getNumberOfCores(){
    return m_systemInfo.dwNumberOfProcessors;
}


/**
 * @brief Gets the cpu architecture
 *
 * @return the architecture on the cpu as an std::string
 */
std::string CpuInformation::getArchitecture(){
    std::string architecture;
    switch((int)m_systemInfo.wProcessorArchitecture){
        case PROCESSOR_ARCHITECTURE_AMD64:
            architecture = "AMD_64";
        break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            architecture = "x86";
        break;
        case PROCESSOR_ARCHITECTURE_IA64:
            architecture = "itanium";
        break;
        case PROCESSOR_ARCHITECTURE_ARM:
            architecture = "ARM";
        break;
        default:
            architecture = "UNKNOWN";
        break;
    }

    return architecture;
}


/**
 * @brief Gets the usage percentage of a cpu core
 * @param core The core number (1, 2, 3 etc)
 * @return The cpu core usage as a double
 */
double CpuInformation::getCoreUsagePercentage(int core){

    PDH_STATUS status = PdhCollectQueryData(m_pdhQuery);
    if(status != ERROR_SUCCESS || status == PDH_INVALID_HANDLE || status == PDH_NO_DATA)
        return 0.0;

//    for(uint i  = 0; i < m_numberOfPdhCounters; i++){
//        PDH_FMT_COUNTERVALUE counterValue;
//        DWORD counterType;
//        status = PdhGetFormattedCounterValue(m_counters[i], PDH_FMT_LONG,0,&counterValue);
//        if(status != ERROR_SUCCESS)
//            m_logger->alert("PdhGetFormattedCounterValue() Failed");
//    }

    return 0.0;
}
