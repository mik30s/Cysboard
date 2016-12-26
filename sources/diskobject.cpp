#include "diskobject.h"

/**
 * @brief DiskObject::DiskObject
 */
DiskObject::DiskObject() {
    try {
        m_ptrDiskInfo = new DiskInformation();
    }
    catch(std::exception &e) {
        throw;
    }
}


/**
 * @brief DiskObject::~DiskObject
 */
DiskObject::~DiskObject() {
    delete m_ptrDiskInfo;
}


/**
 * @brief DiskObject::initialize
 * @return
 */
void DiskObject::initialize() {
}


/**
 * @brief DiskObject::update
 */
void DiskObject::update() {

}

