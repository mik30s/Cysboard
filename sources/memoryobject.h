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

#include <spdlog/spdlog.h>
#include "meminfo.h"


class MemoryObject{

private:
    std::unique_ptr<MemoryInformation> m_ptrMemInfo;
    std::shared_ptr<spdlog::logger> m_logger;

public:
    MemoryObject();
    ~MemoryObject()=default;

    void initialize();
    void update();

    uint64_t m_total;
    uint64_t m_totalSwap;
    uint64_t m_free, m_used;
    double m_freePercentage;
    double m_usedPercentage;
};


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
