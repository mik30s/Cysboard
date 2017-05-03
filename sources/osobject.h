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
#include "iinfoobject.h"

class OsObject
{

private:
    std::unique_ptr<OSInformation> m_osInfo;

public:
    OsObject();
    ~OsObject();

    void initialize();
    void update();

    std::string  m_name;
    std::string  m_vendor;
    std::string  m_version;
    std::string  m_distroName;
    std::string  m_uptime;
    std::string  m_numberOfProcess;

    std::string  getName();
    std::string  getVendor();
    std::string  getVersion();
    std::string  getUptime();
    std::string  getNumberOfProcess();
    std::string  getDistroName();

};

