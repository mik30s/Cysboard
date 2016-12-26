/**
  *
  *
  */

#ifndef CYSBOARD_H
#define CYSBOARD_H

#include "cpuobject.h"
#include "osobject.h"
#include "diskobject.h"
#include "memoryobject.h"

#include <spdlog/spdlog.h> // logging
#include <string>
#include <cstring>
#include <functional>
#ifdef __linux
    #include "sciter/sciter-gtk-main.cpp"
    #include <unistd.h>
#endif


namespace spd = spdlog;

class CysBoard : public sciter::window
{
private:
    std::shared_ptr<spd::logger> m_logger;

    // Information objects for each device
    CpuObject*          m_cpuInfo;
    MemoryObject*       m_ramInfo;
    OsObject*           m_osInfo;
    DiskObject*         m_diskInfo;
    // elemnts in the the dom
    sciter::dom::element root;
    sciter::dom::element memFree;
    sciter::dom::element cpuName;

public:
    CysBoard();
    ~CysBoard();

    void update();

    CpuObject* getCpuInfo();
    MemoryObject* getRamInfo();
    OsObject* getOsInfo();
    DiskObject* getDiskInfo();
};


/**
 * @brief CysBoard::CysBoard
 */
CysBoard::CysBoard() :
    window(SW_MAIN | SW_ALPHA | SW_POPUP, {100, 100, 100, 100})
{
    m_logger = spdlog::get("logger");

    // initialize all objects
    try {
        m_cpuInfo = new CpuObject();
        m_diskInfo = new DiskObject();
        m_osInfo = new OsObject();
        m_ramInfo = new MemoryObject();
    }
    catch(std::exception& ex) {
        m_logger->alert("In CysBoard.cpp -> {0:s}", ex.what());
    }

    m_cpuInfo->initialize();
    m_ramInfo->initialize();
    m_diskInfo->initialize();
    m_osInfo->initialize();

    sciter::dom::element root = sciter::dom::element::root_element(sciter::gview(this->get_hwnd()));
    memFree = root.find_first("#mem_free");
}


CysBoard::~CysBoard(){}


/**
 * @brief Updates all device information
 *
 * Updates all system information by calling
 * each module objects update method. A signal is emitted on each
 * object to notify the theme/qml interface of a change in data.
 * Only objects whose data can change, should be called through update.
 */
void CysBoard::update() {
    // start timer
    while(true){
        usleep(100000);

        m_cpuInfo->update();
        m_ramInfo->update();
        m_diskInfo->update();
        m_osInfo->update();

        m_logger->info("{}","watch" );//memFree.get_value().to_string().c_str());
        memFree.set_text((WCHAR*)std::to_wstring(m_ramInfo->m_free).c_str());
    }
}


/**
 * @brief Gets cpu information object
 * @return A pointer to a CpuInformation Object
 */
CpuObject* CysBoard::getCpuInfo(){ return m_cpuInfo; }


/**
 * @brief CysBoard::getRamInfo
 * @return A Pointer to a RamInformation Object
 */
MemoryObject* CysBoard::getRamInfo(){ return m_ramInfo; }


/**
 * @brief CysBoard::getOsInfo
 * @return A pointer to an OsInformation Object
 */
OsObject* CysBoard::getOsInfo(){ return m_osInfo; }


/**
 * @brief CysBoard::getDiskInfo
 * @return A pointer to a DiskInformation Object
 */
DiskObject* CysBoard::getDiskInfo(){ return m_diskInfo; }



#endif // CYSBOARD_H
