#include "cpuobject.h"


/**
 * @brief CpuObject::CpuObject
 */
CpuObject::CpuObject() {

    m_logger = spdlog::get("cysboardLogger");

    try{
        m_ptrCpuInfo = new CpuInformation();
    }
    catch(std::exception &e){
        //m_logger->alert("{0:s}",e.what());
        throw;
    }
}


/**
 * @brief CpuObject::~CpuObject
 */
CpuObject::~CpuObject(){
    delete m_ptrCpuInfo;

    m_ptrCores.clear();
}


/**
 * @brief CpuObject::operator =
 * @param reg
 * @return
 */
CpuObject& CpuObject::operator=(CpuObject &reg){
    return reg;
}


/**
 * @brief CpuObject::initialize
 * @return
 */
void CpuObject::initialize(){
    m_name = m_ptrCpuInfo->getName();
    m_vendor = m_ptrCpuInfo->getVendor();
    m_architecture = m_ptrCpuInfo->getArchitecture();
    m_numberOfCores = m_ptrCpuInfo->getNumberOfCores();

    for(int core = 0; core < m_numberOfCores; core++){
        m_ptrCores.push_back(new CpuCoreObject());
    }
}


/**
 * @brief CpuObject::update
 */
void CpuObject::update(){
    for(int core = 1; core < m_numberOfCores+1; core++){
        CpuCoreObject* ptrCore = dynamic_cast<CpuCoreObject*>(m_ptrCores[core-1]);

        ptrCore->m_usePercentage = m_ptrCpuInfo->getCoreUsagePercentage(core);
        ptrCore->m_currentSpeed = m_ptrCpuInfo->getCurrentSpeed(core);

        // m_logger->info("{0:f}",ptr->m_currentSpeed);
    }

    m_totalUsagePercent = m_ptrCpuInfo->getTotalUsagePercentage();
}


/**
 * @brief CpuObject::getName
 * @return
 */
std::string CpuObject::getName(){return m_name;}


/**
 * @brief CpuObject::getVendor
 * @return
 */
std::string CpuObject::getVendor(){return m_vendor;}


/**
 * @brief CpuObject::getArchitecture
 * @return
 */
std::string CpuObject::getArchitecture(){return m_architecture;}


/**
 * @brief CpuObject::getNumberOfCores
 * @return
 */
int CpuObject::getNumberOfCores(){return m_numberOfCores;}


/**
 * @brief CpuObject::getTotalUsagePercentage
 * @return
 */
double CpuObject::getTotalUsagePercentage(){return m_totalUsagePercent;}


/**
 * @brief CpuObject::getCores
 * @return
 *
 */
std::vector<CpuCoreObject*> CpuObject::getCores(){return m_ptrCores;}



