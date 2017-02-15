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



#ifndef CYSBOARD_DISKOBJECT_H
#define CYSBOARD_DISKOBJECT_H

#include <memory>
#include "iinfoobject.h"
#include "diskinfo.h"

class DiskObject
{
private:
    std::unique_ptr<DiskInformation> m_ptrDiskInfo;

public:
    DiskObject();
    ~DiskObject()=default;

    void initialize();
    void update();

    void getTotalSpace(const char* disk);
    void getFreeSpace(const char* disk);
    void getUsedSpace(const char* disk);
};

#endif // CYSBOARD_DISKOBJECT_H
