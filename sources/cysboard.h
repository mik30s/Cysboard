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
#include "call_program.h"

#include <spdlog/spdlog.h>  // logging
#include <sys/inotify.h>
#include <string>
#include <cstring>
#include <functional>
#include <memory>
#ifdef __linux
    #include "sciter/sciter-gtk-main.cpp"
    #include <unistd.h>
#endif
#include "util.h"



class CysBoard : public sciter::window
{
private:
    std::shared_ptr<spdlog::logger> m_logger;

    // Information objects for each device
    std::unique_ptr<CpuObject>     m_cpuInfo;
    std::unique_ptr<MemoryObject>  m_ramInfo;
    std::unique_ptr<OsObject>      m_osInfo;
    std::unique_ptr<DiskObject>    m_diskInfo;
    std::unique_ptr<CallProgram>   m_callProgram;

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

    std::vector<sciter::dom::element> m_execNodes;

    double m_updateInterval;

public:
    CysBoard();
    ~CysBoard();

    bool configure();
    void update();
    void destroy();
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
        m_cpuInfo  = std::make_unique<CpuObject>();
        m_diskInfo = std::make_unique<DiskObject>();
        m_osInfo   = std::make_unique<OsObject>();
        m_ramInfo  = std::make_unique<MemoryObject>();
    }
    catch(std::exception& ex) {
        m_logger->alert("In CysBoard.cpp -> {0:s}", ex.what());
    }

    m_callProgram = std::make_unique<CallProgram>();
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

    m_cpuInfo->initialize();
    m_ramInfo->initialize();
    m_diskInfo->initialize();
    m_osInfo->initialize();

    // First initialize references to the DOM
    using namespace sciter;
    m_root = dom::element::root_element(sciter::gview(this->get_hwnd()));
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
    // networking
    // other
    findAllElements(m_root, "[id^=exec_]", m_execNodes);

    // meta tags for other config
    dom::element&& windowPositionX = m_root.find_first("meta[name=position_x]");
    dom::element&& windowPositionY = m_root.find_first("meta[name=position_y]");

    // set window position
    if(windowPositionX.is_valid() && windowPositionY.is_valid()) {
        uint posX, posY;
        DOM_TEXT_TO_NUM(windowPositionX.get_attribute("content"), posX);
        DOM_TEXT_TO_NUM(windowPositionY.get_attribute("content"), posY);
        // move window to position
        // Note: for absolute positioning set margin of <body> to 0 in stylesheet
//        m_logger->info("X {0:d}", posX);
//        m_logger->info("Y {0:d}", posY);
        #ifdef __linux
            gtk_window_move(sciter::gwindow(this->get_hwnd()), posX, posY);
        #endif
    } else retVal = false;

    // set window width and height
    dom::element&& windowWidth = m_root.find_first("meta[name=width]");
    dom::element&& windowHeight = m_root.find_first("meta[name=height]");

    if(windowWidth.is_valid() && windowHeight.is_valid()){
        uint width, height;
        DOM_TEXT_TO_NUM(windowWidth.get_attribute("content"), width);
        DOM_TEXT_TO_NUM(windowHeight.get_attribute("content"), height);

//        m_logger->info("Width {0:d}", width);
//        m_logger->info("Height {0:d}", height);
        #ifdef __linux
            gtk_window_set_resizable(sciter::gwindow(this->get_hwnd()), TRUE);
            gtk_window_resize(sciter::gwindow(this->get_hwnd()), width, height);
        #endif
    }else retVal = false;

    // set update interval
    dom::element&& updateInterval = m_root.find_first("meta[name=time]");
    DOM_TEXT_TO_NUM(updateInterval.get_attribute("content"), m_updateInterval);
    if(m_updateInterval < 0.1) {
        m_updateInterval = 0.1;
    }

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
    string2DomText(m_cpuInfo->m_name, m_cpuName);
    string2DomText(m_cpuInfo->m_architecture, m_cpuArchitecture);
    string2DomText(m_cpuInfo->m_vendor, m_cpuVendor);
    num2DomText(m_cpuInfo->m_numberOfCores, m_cpuNumOfCores);
    num2DomText(m_cpuInfo->m_totalUsagePercent, m_cpuUsage);

    // os values
    string2DomText(m_osInfo->m_name, m_osName);
    string2DomText(m_osInfo->m_distroName, m_osDistroName);
    string2DomText(m_osInfo->m_uptime, m_osUptime);

    // memory values
    num2DomText(m_ramInfo->convert(m_ramInfo->m_total,
                    DOM_TEXT_TO_CSTR(m_memFree.get_attribute("mul"))), m_memTotal);

    num2DomText(m_ramInfo->convert(m_ramInfo->m_free,
                    DOM_TEXT_TO_CSTR(m_memFree.get_attribute("mul"))), m_memFree);

    num2DomText(m_ramInfo->convert(m_ramInfo->m_used,
                    DOM_TEXT_TO_CSTR(m_memFree.get_attribute("mul"))), m_memUsed);
    #ifdef __linux
        num2DomText(m_ramInfo->convert(m_ramInfo->m_totalSwap,
                        DOM_TEXT_TO_CSTR(m_memFree.get_attribute("mul"))), m_memTotalSwap);
    #endif
    // disk values
    // network values
    // execute commands and output result on each update
    for(auto& node: m_execNodes) {
        string2DomText(CallProgram::execute(DOM_TEXT_TO_CSTR(node.get_attribute("cmd"))), node);
    }

    usleep(m_updateInterval * 1000000);
}


/**
 * @brief CysBoard::destroy
 */
void CysBoard::destroy(){
    m_root.destroy();
    // tags for values
    // ram
    m_memFree.destroy();
    m_memUsed.destroy();
    //m_memTotalSwap.destroy();
    //m_memTotal.destroy();
    // cpu
    m_cpuName.destroy();
    m_cpuUsage.destroy();
    m_cpuArchitecture.destroy();
    m_cpuVendor.destroy();
    //m_cpuNumOfCores.destroy();
    // os
    m_osName.destroy();
    m_osDistroName.destroy();
    m_osUptime.destroy();
}

#endif // CYSBOARD_H
