#ifndef CYSBOARD_OSINFO_H
#define CYSBOARD_OSINFO_H

#include <iostream>
#include <cstdio>
#include <sys/utsname.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <map>
#include <cmath>
#include <exception>

#ifdef __linux__
    // defines for fields in /etc/lsb_release
    #define LSB_RELEASE_DISTRO_NAME "DISTRIB_ID"
    #define LSB_RELEASE_NUMBER      "DISTRIB_RELEASE"
    #define LSB_RELEASE_CODE_NAME   "DISTRIB_CODENAME"    // a.k.a nickname
    #define LSB_RELEASE_DESCRIPTION "DISTRIB_DESCRIPTION"

    // defines for fields in /etc/os_release

#endif

class OSInformation
{
private:
    std::shared_ptr<spdlog::logger> m_logger;

#ifdef __linux__
    utsname m_nameInfo;

    FILE* m_fpReleaseCurrent;
    FILE* m_fpLsbRelease;
    FILE* m_fpOsRelease;
    FILE* m_fpUptime;

    static std::map<const char*, const char*> alternateFieldValues;

    std::string readReleaseInfoField(FILE*, const char*);
    const char* getAlternativeFieldName(const char*);

#endif

public:
    enum class UPTIME_FORMAT {
        SECONDS = 0,
        MINUTES = 1,
        HOURS   = 2,
        DAYS    = 3,
        FULL    = 4
    };

    OSInformation();
    ~OSInformation();

    std::string getName();
    std::string getVersion();
    std::string getDistroName();
    std::string getDistroVersion();
    std::string getUptime(UPTIME_FORMAT);

};

#endif // CYSBOARD_OSINFO_H
