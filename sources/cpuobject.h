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

#ifndef CPUOBJECT_H
#define CPUOBJECT_H

#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include "iinfoobject.h"
#include "cpuinfo.h"
#include "cpucoreobject.h"

class CpuObject
{
private:
    std::unique_ptr<CpuInformation> m_ptrCpuInfo;
    std::shared_ptr<spdlog::logger> m_logger;
    std::vector<CpuCoreObject*> m_ptrCores;

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

    std::vector<CpuCoreObject*>& getCores();

    void initialize();
    void update();
};

#endif // CPUOBJECT_H
