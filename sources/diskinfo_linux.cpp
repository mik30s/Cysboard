#include "diskinfo.h"

uint64_t getNthNumber(std::string text, int pos);


/**
 * @brief DiskInformation::DiskInformation
 */
DiskInformation::DiskInformation()
{
    m_dirDisks = opendir(SYSFS_DIRECTORY);

    if(m_dirDisks == nullptr) {
        throw std::runtime_error(std::string(__FUNCTION__) + " failed to open /sys/block");
    }

    std::regex regexp("sd([A-Za-z]|[A-Za-z][0-9])+");

    // get first directory
    auto nextDir = readdir(m_dirDisks);

    while(nextDir != nullptr) {
        if(std::regex_match(nextDir->d_name, regexp)) {
            // get directories subdirectories
            std::string path = std::string(SYSFS_DIRECTORY) + nextDir->d_name;

            auto subDir = opendir(path.c_str());

            std::list<PartitionInfo> subdirs;

            auto nextSubDir = readdir(subDir);

            while(nextSubDir != nullptr) {
                if(std::regex_match(nextSubDir->d_name, regexp)) {
                    // open stat file
                    std::ifstream* statFile = new std::ifstream();
                    statFile->open(getFullPath(nextSubDir->d_name) + "stat");
                    PartitionInfo pInfo = {nextSubDir->d_name, statFile};
                    // add to list
                    subdirs.push_back(pInfo);
                }
                nextSubDir = readdir(subDir);
            }
            m_disksAndPartions.insert({ nextDir->d_name, subdirs });
        }
        nextDir = readdir(m_dirDisks);
    }
}


/**
 * @brief DiskInformation::~DiskInformation
 */
DiskInformation::~DiskInformation() {}


/**
 * @brief DiskInformation::getDiskSize
 * @param diskName
 * @return
 */
uint64_t DiskInformation::getTotalDiskSize(const char* diskName)
{
    std::string diskPath = getFullPath(diskName);

    struct statvfs fs;
    statvfs(diskPath.c_str(), &fs);

    return fs.f_bsize;
}


/**
 * @brief DiskInformation::getDiskSize
 * @param diskName
 * @return
 */
uint64_t DiskInformation::getFreeDiskSize(const char* diskName)
{
    std::string diskPath = getFullPath(diskName);

    struct statvfs fs;
    statvfs(diskPath.c_str(), &fs);

    return fs.f_bfree;
}


/**
 * @brief DiskInformation::getFullPath
 * @param diskName
 * @return
 */
std::string DiskInformation::getFullPath(const char* diskName)
{
    std::stringstream ss;
    std::string diskPath(SYSFS_DIRECTORY);
    std::string diskNameCopy(diskName);

    // determine if its a disk or a partition of a disk
    std::regex regexp("sd[:alpha:]\\d+");
    if(std::regex_match(diskName, regexp)) {
        diskNameCopy[diskNameCopy.size() - 1] = '\0';

        ss << diskNameCopy.c_str() << "/"
           << diskName << "/";

        diskPath += ss.str();
    }
    else {
        diskPath += diskName;
    }

    return diskPath;
}


/**
 * @brief Uses regexp to get the integer value of the Nth number in a string
 * @param text
 * @param pos
 * @return
 */
uint64_t getNthNumber(std::string text, int pos)
{
    const std::regex regx("(?:\\d+\\s+){" + std::to_string(pos) + "}(\\d+).*");

    std::smatch matchingStrings;

    if(std::regex_match(text, matchingStrings, regx)) {
        return (uint64_t)std::stoll(matchingStrings[1]);
    }

    return 0;
}


