#include "osobject.h"


/**
 * @brief OsObject::OsObject
 */
OsObject::OsObject(){
    try{
        m_osInfo = new OSInformation();
    }
    catch(std::exception &ex){ throw; }
}


/**
 * @brief OsObject::initialize
 */
void OsObject::initialize() {
    m_name = m_osInfo->getName();
    m_version = m_osInfo->getVersion();
#ifdef __linux__
    m_distroName = m_osInfo->getDistroName();
#endif
}


/**
 * @brief OsObject::~OsObject
 */
OsObject::~OsObject(){}


/**
 * @brief OsObject::getName
 * @return
 */
std::string OsObject::getName() {
    return m_name;
}


/**
 * @brief OsObject::getVendor
 * @return
 */
std::string OsObject::getVendor() {
    return m_vendor;
}


/**
 * @brief OsObject::getVersion
 * @return
 */
std::string OsObject::getVersion() {
    return m_version;
}


/**
 * @brief OsObject::getUptime
 * @return
 */
std::string OsObject::getUptime() {
    return m_uptime;
}


/**
 * @brief OsObject::getDistroName
 * @return
 */
std::string OsObject::getDistroName() {
    return m_distroName;
}


/**
 * @brief OsObject::update
 */
void OsObject::update() {
    m_uptime = m_osInfo->getUptime(OSInformation::UPTIME_FORMAT::FULL);
}
