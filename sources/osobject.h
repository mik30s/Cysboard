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


#pragma once

#include "osinfo.h"

class OsObject
{

private:
    std::unique_ptr<OSInformation> m_osInfo;

public:
    OsObject();
    ~OsObject() = default;

    void initialize();
    void update();

    std::string  m_name;
    std::string  m_vendor;
    std::string  m_version;
    std::string  m_distroName;
    std::string  m_uptime;
    std::string  m_numberOfProcess;
};

/**
 * @brief OsObject::OsObject
 */
OsObject::OsObject(){
    try{
        m_osInfo = std::make_unique<OSInformation>();
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
 * @brief OsObject::update
 */
void OsObject::update() {
    m_uptime = m_osInfo->getUptime(OSInformation::UPTIME_FORMAT::FULL);
    m_numberOfProcess = m_osInfo->getNumberOfProcesses();
}
