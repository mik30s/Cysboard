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
#include <sstream>
#include <regex>
#include <cstring>
#include <exception>

#ifdef __linux__
    #define MEMORY_TOTAL      "MemTotal"
    #define MEMORY_FREE       "MemFree"
    #define MEMORY_AVAILABLE  "MemAvailable"
    #define MEMORY_SWAP_TOTAL "SwapTotal"
    #define MEMORY_SWAP_FREE  "SwapFree"
    #define MEMORY_CACHED     "Cached"  // not used
    #define MEMORY_BUFFERS    "Buffers"  // not used
#endif


class MemoryInformation{
private:

#ifdef __WIN32
    // ... windows specific stuff
#elif __linux__
    FILE* m_fpInfo;
    uint64_t readMemInfoField(const char*);
#endif

public:
    MemoryInformation();
    ~MemoryInformation();

    std::string getDeviceName();
    uint64_t getTotalAmount();
    uint64_t getFreeAmount();

    double getClockSpeed();

#ifdef __WIN32

#elif __linux__
    uint64_t getTotalSwapAmount();
    uint64_t getFreeSwapAmount();

#endif

};

