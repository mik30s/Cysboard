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


#ifndef CYSBOARD_CPUINFO_H
#define CYSBOARD_CPUINFO_H

#include <exception>
#include "ccpuid.h"        // a wrapper around cpuid for cpu information
#ifdef __linux__
    #include <sys/utsname.h>
#elif _WIN32
    #include "win32util.h"
#endif

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <cassert>

#ifdef __linux__
    #define MAX_CPU_FIELDS 10
    #define BUF_SIZE 1024
    #define MAX_CPU 16
    #define CPUINFO_SPEED "cpu MHZ"
    #define CPUINFO_PROCESSOR "processor"
    #define CPUINFO_CACHE_SIZE "cache size"
#endif


class CpuInformation
{
private:
    static std::vector<std::string> m_vendorNames;
    std::shared_ptr<spdlog::logger> m_logger;
    CCPUID m_cpuid;
    int m_numOfCores;

#ifdef __linux__
    FILE* m_fpInfo;
    FILE* m_fpStats;

    struct CpuTicks {
        uint64_t fields[MAX_CPU_FIELDS];
        uint64_t totalTick[MAX_CPU];
        uint64_t totalTickOld[MAX_CPU];
        uint64_t idle[MAX_CPU];
        uint64_t idleOld[MAX_CPU];
        uint64_t delTotalTick[MAX_CPU];
        uint64_t delIdle[MAX_CPU];
    } m_ticks;

    std::string readCpuInfoField(int core, std::string fieldName);

#elif _WIN32
    PDH_HQUERY m_pdhQuery;
    SYSTEM_INFO m_systemInfo;

    int m_numberOfPdhCounters;
    std::vector<PDH_COUNTER_PATH_ELEMENTS> m_counterPaths;
    std::vector<PDH_HCOUNTER> m_counters;

#endif

public:
    CpuInformation();
    ~CpuInformation();

    std::string getName();
    std::string getVendor();
    std::string getArchitecture();
    int    getNumberOfCores();
    double getTotalUsagePercentage();
    double getCoreUsagePercentage(int core);
    double getCurrentSpeed(int core);
    double getCacheSize(int core);
};

#endif // CYSBOARD_CPUINFO_H

