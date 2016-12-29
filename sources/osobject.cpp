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
