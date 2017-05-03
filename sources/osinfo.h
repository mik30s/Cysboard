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

#include <iostream>
#include <cstdio>
#include <sys/utsname.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <map>
#include <cmath>
#include <exception>
#include "util.h"

#ifdef __linux__
    // defines for fields in /etc/lsb_release
    #define LSB_RELEASE_DISTRO_NAME "DISTRIB_ID"
    #define LSB_RELEASE_NUMBER      "DISTRIB_RELEASE"
    #define LSB_RELEASE_CODE_NAME   "DISTRIB_CODENAME"    // a.k.a nickname
    #define LSB_RELEASE_DESCRIPTION "DISTRIB_DESCRIPTION"

    // defines for fields in /etc/os_release

#endif

class OSInformation
{
private:
    std::shared_ptr<spdlog::logger> m_logger;

#ifdef __linux__
    utsname m_nameInfo;

    FILE* m_fpReleaseCurrent;
    FILE* m_fpLsbRelease;
    FILE* m_fpOsRelease;
    FILE* m_fpUptime;
    DIR* m_procDir;

    static std::map<const char*, const char*> alternateFieldValues;

    std::string readReleaseInfoField(FILE*, const char*);
    const char* getAlternativeFieldName(const char*);

#endif

public:
    enum class UPTIME_FORMAT {
        SECONDS = 0,
        MINUTES = 1,
        HOURS   = 2,
        DAYS    = 3,
        FULL    = 4
    };

    OSInformation();
    ~OSInformation();

    std::string getName();
    std::string getVersion();
    std::string getDistroName();
    std::string getDistroVersion();
    std::string getNumberOfProcesses();
    std::string getUptime(UPTIME_FORMAT);

};
