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



/**
 *  This is the central class of the program. It sets up the
 *  Document Object Model throught the Sciter Engine and sets references
 *  to DOM nodes for values to be diplayed.
 */

#ifndef CYSBOARD_H
#define CYSBOARD_H

#include "cpuobject.h"
#include "osobject.h"
#include "diskobject.h"
#include "memoryobject.h"

#include <spdlog/spdlog.h>  // logging
#include <sys/inotify.h>
#include <string>
#include <cstring>
#include <functional>
#ifdef __linux
    #include "sciter/sciter-gtk-main.cpp"
    #include <unistd.h>
#endif

#define INT_TO_DOM_TEXT(source, destination) \
            if(destination.is_valid()) { \
                destination.set_text((const WCHAR*)aux::utf2w(std::to_string(source))); \
            }

#define STR_TO_DOM_TEXT(source, destination)\
            if(destination.is_valid()) { \
                destination.set_text((const WCHAR*)aux::utf2w(source)); \
            }

#define DOM_TEXT_TO_INT(source, destination)\
            destination = aux::wtoi(source.c_str()); \


class CysBoard : public sciter::window
{
private:
    std::shared_ptr<spdlog::logger> m_logger;

    // Information objects for each device
    CpuObject*          m_cpuInfo;
    MemoryObject*       m_ramInfo;
    OsObject*           m_osInfo;
    DiskObject*         m_diskInfo;
    // references to elements in the the DOM
    sciter::dom::element m_root;
    // ram dom references
    sciter::dom::element m_memFree;
    sciter::dom::element m_memUsed;
    sciter::dom::element m_memTotal;
    sciter::dom::element m_memTotalSwap;
    // cpu dom references
    sciter::dom::element m_cpuName;
    sciter::dom::element m_cpuUsage;
    sciter::dom::element m_cpuVendor;
    sciter::dom::element m_cpuArchitecture;
    sciter::dom::element m_cpuNumOfCores;
    // os dom references
    sciter::dom::element m_osName;
    sciter::dom::element m_osDistroName;
    sciter::dom::element m_osUptime;

    double m_updateInterval;

public:
    CysBoard();
    ~CysBoard();

    bool configure();
    void update();

    CpuObject*    getCpuInfo();
    MemoryObject* getRamInfo();
    OsObject*     getOsInfo();
    DiskObject*   getDiskInfo();
};


/**
 * @brief CysBoard::CysBoard
 */
CysBoard::CysBoard() :
    window(SW_MAIN | SW_ALPHA | SW_POPUP, {0, 0, 10, 10})
{
    m_logger = spdlog::get("logger");

    // initialize all objects
    try {
        m_cpuInfo  = new CpuObject();
        m_diskInfo = new DiskObject();
        m_osInfo   = new OsObject();
        m_ramInfo  = new MemoryObject();
    }
    catch(std::exception& ex) {
        m_logger->alert("In CysBoard.cpp -> {0:s}", ex.what());
    }

    m_cpuInfo->initialize();
    m_ramInfo->initialize();
    m_diskInfo->initialize();
    m_osInfo->initialize();
}


/**
 * @brief CysBoard::~CysBoard
 */
CysBoard::~CysBoard(){}


/**
 * @brief Configures cysboard according to settings found in <meta> tags
 *        The window position and dimension are mandatory, others are optional
 * @return True if success or False is failed
 */
bool CysBoard::configure() {
    bool retVal = true;
    // First initialize references to the DOM
    using namespace sciter;
    dom::element m_root = dom::element::root_element(sciter::gview(this->get_hwnd()));
    // tags for values
    // ram
    m_memFree = m_root.find_first("#mem_free");
    m_memUsed = m_root.find_first("#mem_used");
    m_memTotalSwap = m_root.find_first("#mem_swap_total");
    m_memTotal = m_root.find_first("#mem_total");
    // cpu
    m_cpuName = m_root.find_first("#cpu_name");
    m_cpuUsage = m_root.find_first("#cpu_usage");
    m_cpuArchitecture = m_root.find_first("#cpu_arch");
    m_cpuVendor = m_root.find_first("#cpu_vendor");
    m_cpuNumOfCores = m_root.find_first("#cpu_num_cores");
    // os
    m_osName  = m_root.find_first("#os_name");
    m_osDistroName = m_root.find_first("#os_distro_name");
    m_osUptime = m_root.find_first("#os_uptime");
    // disk

    // meta tags for other config
    dom::element windowPositionX = m_root.find_first("meta[name=position_x]");
    dom::element windowPositionY = m_root.find_first("meta[name=position_y]");

    // set window position
    if(windowPositionX.is_valid() && windowPositionY.is_valid()) {
        uint posX, posY;
        DOM_TEXT_TO_INT(windowPositionX.get_attribute("content"), posX);
        DOM_TEXT_TO_INT(windowPositionY.get_attribute("content"), posY);
        // move window to position
        // Note: for absolute positioning set margin of <body> to 0 in stylesheet
//        m_logger->info("X {0:d}", posX);
//        m_logger->info("Y {0:d}", posY);
        #ifdef __linux
            gtk_window_move(sciter::gwindow(this->get_hwnd()), posX, posY);
        #endif
    } else retVal = false;

    // set window width and height
    dom::element windowWidth = m_root.find_first("meta[name=width]");
    dom::element windowHeight = m_root.find_first("meta[name=height]");

    if(windowWidth.is_valid() && windowHeight.is_valid()){
        uint width, height;
        DOM_TEXT_TO_INT(windowWidth.get_attribute("content"), width);
        DOM_TEXT_TO_INT(windowHeight.get_attribute("content"), height);

//        m_logger->info("Width {0:d}", width);
//        m_logger->info("Height {0:d}", height);
        #ifdef __linux
            gtk_window_set_resizable(sciter::gwindow(this->get_hwnd()), TRUE);
            gtk_window_resize(sciter::gwindow(this->get_hwnd()), width, height);
        #endif
    }else retVal = false;

    // set update interval
    dom::element updateInterval = m_root.find_first("meta[name=time]");
    DOM_TEXT_TO_INT(updateInterval.get_attribute("content"), m_updateInterval);
    if(m_updateInterval < 0.1) {
        m_updateInterval = 0.1;
    }

    // info sources that have options


    return retVal;
}


/**
 * @brief Updates information from all sources
 *
 * Updates all system information by calling
 * each module objects update method. A signal is emitted on each
 * object to notify the theme/qml interface of a change in data.
 * Only objects whose data can change, should be called through update.
 */
void CysBoard::update() {
    m_cpuInfo->update();
    m_ramInfo->update();
    m_diskInfo->update();
    m_osInfo->update();

    // convert values and update
    // cpu values
    STR_TO_DOM_TEXT(m_cpuInfo->m_name, m_cpuName);
    INT_TO_DOM_TEXT(m_cpuInfo->m_totalUsagePercent, m_cpuUsage);
    STR_TO_DOM_TEXT(m_cpuInfo->m_architecture, m_cpuArchitecture);
    STR_TO_DOM_TEXT(m_cpuInfo->m_vendor, m_cpuVendor);
    INT_TO_DOM_TEXT(m_cpuInfo->m_numberOfCores, m_cpuNumOfCores);
    // os values
    STR_TO_DOM_TEXT(m_osInfo->m_name, m_osName);
    STR_TO_DOM_TEXT(m_osInfo->m_distroName, m_osDistroName);
    STR_TO_DOM_TEXT(m_osInfo->m_uptime, m_osUptime);
    // memory values
    INT_TO_DOM_TEXT(m_ramInfo->m_free, m_memFree);
    INT_TO_DOM_TEXT(m_ramInfo->m_used, m_memUsed);
    #ifdef __linux
        INT_TO_DOM_TEXT(m_ramInfo->m_totalSwap, m_memTotalSwap);
    #endif
    // disk values
    // network values

    usleep(m_updateInterval * 1000000);
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
