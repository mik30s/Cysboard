#ifndef CYSBOARD_DISKINFO_H
#define CYSBOARD_DISKINFO_H

// TODO: DONT forget to clean up unnecessary headers
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <string>
#include <regex>
#include <cstdint>
#include <string>

#ifdef __linux__
    #define SYSFS_DIRECTORY "/sys/block/"
#endif

class DiskInformation
{
private:
#ifdef __linux__
    DIR* m_dirDisks;

    // holds information for a single partition
    struct PartitionInfo {
        std::string name; // name of a partition
        std::ifstream* statFile; // a reference to its stat file
    };

    std::map<std::string, std::list<PartitionInfo>> m_disksAndPartions;

    std::string getFullPath(const char* diskName);

#endif

public:
    DiskInformation();
    ~DiskInformation();

    std::list<std::string>& getPartitions(const char* diskName);
    uint64_t getTotalDiskSize(const char* diskName);
    uint64_t getFreeDiskSize(const char* diskName);
};

#endif // CYSBOARD_DISKINFO_H
