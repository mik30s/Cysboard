#ifndef CYSBOARD_CCPUID_H
#define CYSBOARD_CCPUID_H

/**
 * @brief A warpper around cpuid instruction for intel based cpus
 *
 * I only defined this because __get_cpuid() provided by GCC for some reason
 * returned the vendor name as model name on my machine. I was not sure why.
 */

#include <cstdint>

class CCPUID
{
private:
    uint32_t registers[4]; // holds the registers returned by cpuid on x86 processors
                           // EAX  = registers[0], EBX = registers[1],
                           // ECX = registers[2], EDX = registers[3]
public:
    enum CPUID_INSRUCTION : long long {
        PROCESSOR_VENDOR            = 0x0,
        PROCESSOR_SERIAL_NUMBER     = 0x30000000,
        PROCESSOR_HIGHEST_FUNCTION  = 0x80000000,
        PROCESSOR_FEATURE_BITS      = 0x80000001,
        PROCESSOR_BRAND             = 0x80000002
    };

    /**
     * @brief CCpuId constructor
     */
    CCPUID(){}

    /**
     * @brief execute
     * @param i The instruction parameter
     */
    void execute(long long i){
#ifdef _WIN32
       // use win32 provided function
       __cpuid((int*)registers, i);
#else
       // for linux and others
       asm volatile (
        "cpuid": "=a" (registers[0]),
                 "=b" (registers[1]),
                 "=c" (registers[2]),
                 "=d" (registers[3])
               : "a" (i),
                 "c" (0)
       );
#endif
    }

    const uint32_t& getEAX() const {return registers[0];}
    const uint32_t& getEBX() const {return registers[1];}
    const uint32_t& getECX() const {return registers[2];}
    const uint32_t& getEDX() const {return registers[3];}
};

#endif // CCPUID_H
