#ifndef OSOBJECT_H
#define OSOBJECT_H

#include "osinfo.h"

class OsObject
{

private:
    OSInformation* m_osInfo;

public:
    OsObject();
    ~OsObject();

    void initialize();
    void update();

    std::string m_name;
    std::string  m_vendor;
    std::string  m_version;
    std::string  m_distroName;
    std::string  m_uptime;

    std::string  getName();
    std::string  getVendor();
    std::string  getVersion();
    std::string  getUptime();
    std::string  getDistroName();

};

#endif // OSOBJECT_H
