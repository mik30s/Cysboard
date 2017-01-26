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


#include "memoryobject.h"



/**
 * @brief MemoryObject::MemoryObject
 */
MemoryObject::MemoryObject(){
    m_logger = spdlog::get("cysboardLogger");

    try{
        m_ptrMemInfo = std::make_unique<MemoryInformation>();
    }
    catch(std::exception& ex){
        throw;
    }
}


/**
 * @brief MemoryObject::~MemoryObject
 */
//MemoryObject::~MemoryObject(){}


/**
 * @brief MemoryObject::initialize
 * @return
 */
void MemoryObject::initialize(){
    m_total = m_ptrMemInfo->getTotalAmount();

#ifdef __linux__
    m_totalSwap = m_ptrMemInfo->getTotalSwapAmount();

#endif
    // m_logger->info("Total memory {0:d}", m_total);
}


/**
 * @brief MemoryObject::update
 */
void MemoryObject::update(){
    m_free = m_ptrMemInfo->getFreeAmount();
    m_used = m_total - m_free;

    return;
}


/**
 * @brief MemoryObject::getTotal
 * @return
 */
uint64_t MemoryObject::getTotal(){
    return m_total;
}


/**
 * @brief MemoryObject::getFree
 * @return
 */
uint64_t MemoryObject::getFree(){
    return m_free;
}


/**
 * @brief MemoryObject::getUsed
 * @return
 */
uint64_t MemoryObject::getUsed(){
    return m_used;
}


/**
 * @brief MemoryObject::getFreePercentage
 * @return
 */
double MemoryObject::getFreePercentage(){
    return m_freePercentage;
}


/**
 * @brief MemoryObject::getUsedPercentage
 * @return
 */
double MemoryObject::getUsedPercentage(){
    return m_usedPercentage;
}

/**
 * @brief Converts the value using the specified multiplier
 * @param value the value to convert
 * @param multiplier MB, KB or GB
 * @return
 */
double MemoryObject::convert(uint64_t value, const char* multiplier){
    double retVal =  0;
    const uint64_t MEGABYTE = 1048576;
    const uint64_t KILOBYTE = 1024;

    if(std::strncmp("KB", multiplier, 2) == 0){
        return value;
    }
    if(std::strncmp("MB", multiplier, 2) == 0){
        retVal = (double) value / KILOBYTE;
    }
    if(std::strncmp("GB", multiplier, 2) == 0){
        retVal = (double) value / MEGABYTE;
    }

    return std::floor(retVal * 100) / 100;
}
