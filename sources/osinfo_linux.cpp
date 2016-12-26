#include "osinfo.h"


std::map<const char*, const char*>
OSInformation::alternateFieldValues = {
    {"DISTRIB_ID",          "NAME"},
    {"DISTRIB_RELEASE",     "VERSION_ID"},
    {"DISTRIB_CODENAME",    "VERSION"},
    {"DISTRIB_DESCRIPTION", "PRETTY_NAME"}
};

/**
 * @brief OSInformation::OSInformation
 */
OSInformation::OSInformation(){
    m_logger = spdlog::get("cysboardLogger");

    uname(&m_nameInfo);

    // /etc/oracle-release /etc/redhat-realease
    m_fpLsbRelease = fopen("/etc/lsb-release","r");
    m_fpOsRelease = fopen("/etc/os-release", "r");
    m_fpUptime = fopen("/proc/uptime","r");

    if(m_fpLsbRelease == nullptr && m_fpOsRelease == nullptr){
        throw std::runtime_error(std::string(__FUNCTION__) + "Failed to open /etc/lsb-release" \
                                              " and /etc/os-release");
    }

    m_fpReleaseCurrent = m_fpLsbRelease;
}


/**
 * @brief OSInformation::~OSInformation
 */
OSInformation::~OSInformation(){
    fclose(m_fpLsbRelease);
    fclose(m_fpOsRelease);
    fclose(m_fpUptime);
}


/**
 * @brief OSInformation::getName
 * @return
 */
std::string OSInformation::getName(){
    return std::string(m_nameInfo.sysname);
}


/**
 * @brief OSInformation::getVersion
 * @return
 */
std::string OSInformation::getVersion(){
    return std::string(m_nameInfo.release);
}


/**
 * @brief OSInformation::getDistroName
 * @return
 */
std::string OSInformation::getDistroName(){
    const char* fieldName = getAlternativeFieldName(LSB_RELEASE_DISTRO_NAME);
    return readReleaseInfoField(m_fpReleaseCurrent, fieldName);
}


/**
 * @brief OSInformation::getDistroVersion
 * @return
 */
std::string OSInformation::getDistroVersion(){
    const char* fieldName = getAlternativeFieldName(LSB_RELEASE_NUMBER);
    return readReleaseInfoField(m_fpReleaseCurrent, fieldName);
}


/**
 * @brief Gets the Systems uptime from last reboot
 *        in the following format DDD:HH:MM:SS
 * @return Returns the uptime in an std::string
 */
std::string OSInformation::getUptime(UPTIME_FORMAT format = UPTIME_FORMAT::FULL) {
    std::stringstream sstream;
    std::string retValue;
    unsigned long long seconds= 0, mins = 0, hrs = 0, days = 0;
    char line[20];

    if(m_fpUptime != nullptr){
        fseek(m_fpUptime, 0, SEEK_SET);
        if(fgets(line, sizeof(line), m_fpUptime) != nullptr){
            sstream << line;
            sstream >> seconds;

            // conversions
            mins = std::floor(seconds / 60);
            hrs = std::floor(mins / 60);
            days = std::floor( hrs / 24);

            // clear stream first
            sstream.str("");
            switch(format){
                case UPTIME_FORMAT::SECONDS:
                    sstream << seconds;
                    break;
                case UPTIME_FORMAT::MINUTES:
                    sstream << mins;
                    break;
                case UPTIME_FORMAT::HOURS:
                    sstream << hrs;
                    break;
                case UPTIME_FORMAT::DAYS:
                    sstream << days;
                    break;
                case UPTIME_FORMAT::FULL:
                default:
                    sstream << days << ":" << hrs % 24
                            << ":" << mins % 60
                            << ":" << (long long) seconds % 60;
                    break;
            }

            sstream >> retValue;
        }
    }

    return retValue;
}


/**
 * @brief readReleaseInfoField
 * @return
 */
std::string OSInformation::readReleaseInfoField(FILE* fp, const char* fieldName){
    char line[30], *ptrMatch;
    std::string value, field;

    while(fgets(line, sizeof(line), fp) != nullptr){
        if((ptrMatch = strstr(line, fieldName)) != nullptr){
            ptrMatch = strstr(line, "=");
            if(ptrMatch != nullptr){
                ptrMatch += 1;
                value = std::string(ptrMatch);
                break;
            }
        }
    }

    // remove quotes if any
    if(value.front() == '"'){
        value.erase(0,1); // remove first
        value.erase(value.size() - 1); // remove last
    }

    return value;
}


/**
 * @brief OSInformation::getAlternativeFieldName
 * @param name
 * @return
 */
const char* OSInformation::getAlternativeFieldName(const char* name){
    if(m_fpLsbRelease == nullptr) {
        m_fpReleaseCurrent = m_fpLsbRelease;
        return alternateFieldValues[name];
    }
    else return name;
}

