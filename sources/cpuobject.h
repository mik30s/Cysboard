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

#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include "cpuinfo.h"
#include "cpucoreobject.h"

class CpuObject
{
private:
    std::unique_ptr<CpuInformation> m_ptrCpuInfo;
    std::shared_ptr<spdlog::logger> m_logger;
    std::vector<std::unique_ptr<CpuCoreObject>> m_ptrCores;

public:
    CpuObject();
    ~CpuObject();

    std::string m_name;
    std::string m_vendor;
    std::string m_architecture;

    int m_numberOfCores;
    double m_totalUsagePercent;

    std::string getName();
    std::string getVendor();
    std::string getArchitecture();

    int getNumberOfCores();
    double getTotalUsagePercentage();

    std::vector<std::unique_ptr<CpuCoreObject>>& getCores();

    void initialize();
    void update();
};

// simple float rounding macro
#define ROUND(value)\
    std::floor(value * 10 + 0.5) / 10;\


/**
 * @brief CpuObject::CpuObject
 */
CpuObject::CpuObject() {

    m_logger = spdlog::get("cysboardLogger");

    try{
        m_ptrCpuInfo = std::make_unique<CpuInformation>();
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
        m_ptrCores.push_back(std::move(std::make_unique<CpuCoreObject>()));
    }
}


/**
 * @brief CpuObject::update
 */
void CpuObject::update(){
    for(int core = 1, i = 0; core < m_numberOfCores + 1; core++, i++){
        m_ptrCores[i]->m_usePercentage = std::ceil(m_ptrCpuInfo->getCoreUsagePercentage(core));
        m_ptrCores[i]->m_currentSpeed = std::ceil(m_ptrCpuInfo->getCurrentSpeed(core));
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
std::vector<std::unique_ptr<CpuCoreObject>>&
CpuObject::getCores(){return m_ptrCores;}


