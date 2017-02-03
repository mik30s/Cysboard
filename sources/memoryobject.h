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


#ifndef MEMORYOBJECT_H
#define MEMORYOBJECT_H

#include "iinfoobject.h"
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

    uint64_t getTotal();
    uint64_t getFree();
    uint64_t getUsed();
    double getFreePercentage();
    double getUsedPercentage();
};

#endif // MEMORYOBJECT_H
