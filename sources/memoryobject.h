#ifndef MEMORYOBJECT_H
#define MEMORYOBJECT_H

#include "iinfoobject.h"
#include <spdlog/spdlog.h>
#include "meminfo.h"

class MemoryObject : public IInfoObject{

private:
    MemoryInformation* m_ptrMemInfo;
    std::shared_ptr<spdlog::logger> m_logger;

public:
    MemoryObject();
    ~MemoryObject();

    void initialize();
    void update();

    std::string m_deviceName;

    uint64_t m_total;
    uint64_t m_totalSwap;
    uint64_t m_free, m_used;
    double m_freePercentage;
    double m_usedPercentage;

    uint64_t getTotal();
    uint64_t getFree();
    uint64_t getUsed();
    double getFreePercentage();
    double getUsedPercentage();

    std::string  getDeviceName();
};

#endif // MEMORYOBJECT_H
