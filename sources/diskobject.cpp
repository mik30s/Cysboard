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



#include "diskobject.h"

/**
 * @brief DiskObject::DiskObject
 */
DiskObject::DiskObject() {
    try {
        m_ptrDiskInfo = std::make_unique<DiskInformation>();
    }
    catch(std::exception &e) {
        throw;
    }
}


/**
 * @brief DiskObject::initialize
 * @return
 */
void DiskObject::initialize() {
    // working on this
}


/**
 * @brief DiskObject::update
 */
void DiskObject::update() {
    // working on this
}

