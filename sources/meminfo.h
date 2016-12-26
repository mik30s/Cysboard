#ifndef CYSBOARD_MEMINFO_H
#define CYSBOARD_MEMINFO_H

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

#endif // CYSBOARD_MEMINFO_H
