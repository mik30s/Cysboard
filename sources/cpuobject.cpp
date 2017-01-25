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



#include "cpuobject.h"

// simple float rounding macro
#define ROUND(value)\
    std::floor(value * 10 + 0.5) / 10;\


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

        ptrCore->m_usePercentage = std::ceil(m_ptrCpuInfo->getCoreUsagePercentage(core));
        ptrCore->m_currentSpeed = std::ceil(m_ptrCpuInfo->getCurrentSpeed(core));

        // m_logger->info("{0:f}",ptr->m_currentSpeed);
    }

    m_totalUsagePercent = ROUND(m_ptrCpuInfo->getTotalUsagePercentage());
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



