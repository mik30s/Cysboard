#ifndef CPUCOREOBJECT_H
#define CPUCOREOBJECT_H

#include "iinfoobject.h"

class CpuCoreObject
{
public:
    CpuCoreObject();
    ~CpuCoreObject();

    double m_currentSpeed;
    int m_cacheSize;
    double m_usePercentage;

    int getCurrentSpeed();
    int getCacheSize();
    double getUsePercentage();
};

#endif // CPUCOREOBJECT_H
