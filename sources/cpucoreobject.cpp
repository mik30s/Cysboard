#include "cpucoreobject.h"

CpuCoreObject::CpuCoreObject(){}
CpuCoreObject::~CpuCoreObject(){}

int CpuCoreObject::getCurrentSpeed(){
    return m_currentSpeed;
}

int CpuCoreObject::getCacheSize(){
    return m_cacheSize;
}

double CpuCoreObject::getUsePercentage(){
    return m_usePercentage;
}

