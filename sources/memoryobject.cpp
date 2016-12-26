#include "memoryobject.h"

/**
 * @brief MemoryObject::MemoryObject
 */
MemoryObject::MemoryObject(){
    m_logger = spdlog::get("cysboardLogger");

    try{
        m_ptrMemInfo = new MemoryInformation();
    }
    catch(std::exception& ex){
        throw;
    }
}


/**
 * @brief MemoryObject::~MemoryObject
 */
MemoryObject::~MemoryObject(){}


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
 * @brief Get the RAM chip name
 *
 * Not yet implemented.
 *
 * @return
 */
std::string MemoryObject::getDeviceName(){
    return "";
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
