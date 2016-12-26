#ifndef CPUOBJECT_H
#define CPUOBJECT_H

#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include "iinfoobject.h"
#include "cpuinfo.h"
#include "cpucoreobject.h"

class CpuObject : public IInfoObject{
    CpuInformation* m_ptrCpuInfo;
    std::shared_ptr<spdlog::logger> m_logger;

public:
    CpuObject();
    ~CpuObject();
    CpuObject(CpuObject&);
    CpuObject& operator=(CpuObject &);

    std::string m_name;
    std::string m_vendor;
    std::string m_architecture;

    int m_numberOfCores;
    double m_totalUsagePercent;

    std::vector<CpuCoreObject*> m_ptrCores;

    std::string getName();
    std::string getVendor();
    std::string getArchitecture();

    int getNumberOfCores();
    double getTotalUsagePercentage();

    std::vector<CpuCoreObject*> getCores();

    void initialize();
    void update();
};

#endif // CPUOBJECT_H
