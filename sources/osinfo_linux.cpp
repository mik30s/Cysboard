/*Copyright 2016 Michael Osei

This file is part of Cysboard.

Cysbaord is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cysboard is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cysboard.  If not, see <http://www.gnu.org/licenses/>.*/


/**
  *
  *
  */
#include "osinfo.h"


std::map<const char*, const char*>
OSInformation::alternateFieldValues = {
    {LSB_RELEASE_DISTRO_NAME,   "NAME"},
    {LSB_RELEASE_NUMBER,        "VERSION_ID"},
    {LSB_RELEASE_CODE_NAME,     "VERSION"},
    {LSB_RELEASE_DESCRIPTION,   "PRETTY_NAME"}
};

/**
 * @brief Creats an OsInformation object
 *        Also opens files for various information sources
 */
OSInformation::OSInformation(){
    m_logger = spdlog::get("logger");

    uname(&m_nameInfo);

    // /etc/oracle-release /etc/redhat-realease
    m_fpLsbRelease = fopen("/etc/lsb-release","r");
    m_fpOsRelease = fopen("/etc/os-release", "r");
    m_fpUptime = fopen("/proc/uptime","r");
    m_procDir = opendir("/proc");

    if(m_fpLsbRelease == nullptr && m_fpOsRelease != nullptr){
        m_fpLsbRelease = m_fpOsRelease;
    }
    else if(m_fpLsbRelease != nullptr && m_fpOsRelease == nullptr){
        m_fpOsRelease = m_fpLsbRelease;
    }
    else if(m_fpLsbRelease == nullptr && m_fpOsRelease == nullptr) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 " Failed to open /etc/lsb-release" \
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
std::string OSInformation::getVersion() {
    return std::string(m_nameInfo.release);
}


/**
 * @brief OSInformation::getDistroName
 * @return
 */
std::string OSInformation::getDistroName() {
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
    retValue.reserve(128);
    unsigned long long seconds= 0, mins = 0, hrs = 0, days = 0;

    if(m_fpUptime != nullptr) {
        fseek(m_fpUptime, 0, SEEK_SET);

        if(fread(const_cast<char*>(retValue.data()), 1, retValue.capacity(), m_fpUptime)){
            retValue += retValue.data();
            sstream << retValue;
            sstream >> seconds;
            // convert time
            mins = std::floor(seconds / 60);
            hrs  = std::floor(mins / 60);
            days = std::floor(hrs / 24);

            sstream.str("");
            sstream.clear();
            switch(format) {
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

std::string OSInformation::getNumberOfProcesses(){
    unsigned long number = 0;
    if (m_procDir != nullptr) {
        // count number of subdirs that have only numbers in their name,
        std::regex regex("[[:digit:]]+");
        auto nextDir = readdir(m_procDir);

        while (nextDir != nullptr) {
            if (std::regex_match(nextDir->d_name, regex)) {
                number++;
            }
            nextDir = readdir(m_procDir);
        }
    }
    // rewind when done
    rewinddir(m_procDir);

    return std::to_string(number);
}


/**
 * @brief readReleaseInfoField
 * @return
 */
std::string OSInformation::readReleaseInfoField(FILE* fp, const char* fieldName){
    char line[30], *ptrMatch;
    std::string value, field;

    while(fgets(line, sizeof(line), fp) != nullptr) {
        if((ptrMatch = strstr(line, fieldName)) != nullptr) {
            ptrMatch = strstr(line, "=");
            if(ptrMatch != nullptr) {
                ptrMatch += 1;
                value = std::string(ptrMatch);
                break;
            }
        }
    }

    // remove quotes if any
    if(value.front() == '"') {
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

