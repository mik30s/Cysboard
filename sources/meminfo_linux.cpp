/**
 * Provides functions for gathering/retrieving memory information on a linux system.
 * The constructor opens the /proc/meminfo files for
 * reading memory information. If that fails an exception is thrown.
 *
 *
 */
#include "meminfo.h"


/**
 * @brief MemoryInformation::MemoryInformation
 */
MemoryInformation::MemoryInformation() {
    m_fpInfo = fopen("/proc/meminfo", "r");

    if(m_fpInfo == nullptr) {
        throw std::runtime_error( std::string(__FUNCTION__) + "Failed to open /proc/meminfo");
    }
}


/**
 * @brief MemoryInformation::~MemoryInformation
 */
MemoryInformation::~MemoryInformation() {
    fclose(m_fpInfo);
}


/**
 * @brief MemoryInformation::readMemInfoField
 * @return
 */
uint64_t MemoryInformation::readMemInfoField(const char* fieldName) {
    std::stringstream ss;
    std::string fieldValue;
    char line[30], *ptrMatch;

    fseek(m_fpInfo, 0, SEEK_SET);
    fflush(m_fpInfo);

    while(fgets(line, sizeof(line), m_fpInfo) != nullptr) {
        if((ptrMatch = strstr(line, fieldName)) != nullptr) {
            std::string matchedString(ptrMatch);
            // find the first value of the seperator
            size_t endPos = matchedString.find_first_of(":");
            // add 2 to endPos because there is space between
            // fieldName and value
            fieldValue = matchedString.substr(endPos+2);

            // extract number from string
            fieldValue = std::regex_replace(fieldValue, std::regex("[A-Za-z\\s]"), std::string(""));

            break;
        }
    }

    // convert to uint64_t
    uint64_t value;
    ss << fieldValue;
    ss >> value;

    fflush(m_fpInfo);

    return value;
}


/**
 * @brief MemoryInformation::getTotalAmount
 * @return
 */
uint64_t MemoryInformation::getTotalAmount(){
    return readMemInfoField(MEMORY_TOTAL);
}


/**
 * @brief MemoryInformation::getFreeAmount
 * @return
 */
uint64_t MemoryInformation::getFreeAmount(){
    return readMemInfoField(MEMORY_FREE);
}


/**
 * @brief MemoryInformation::getTotalSwapAmount
 * @return
 */
uint64_t MemoryInformation::getTotalSwapAmount(){
    return readMemInfoField(MEMORY_SWAP_TOTAL);
}


/**
 * @brief MemoryInformation::getFreeSwapAmount
 * @return
 */
uint64_t MemoryInformation::getFreeSwapAmount(){
    return readMemInfoField(MEMORY_SWAP_FREE);
}




