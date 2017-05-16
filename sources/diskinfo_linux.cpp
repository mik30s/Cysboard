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
along with Cysboard.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "diskinfo.h"

/**
 * @brief DiskInformation::DiskInformation
 */
DiskInformation::DiskInformation()
    :m_dirDisks(opendir(SYSFS_DIRECTORY))
{
    if(m_dirDisks == nullptr) {
        throw std::runtime_error(std::string(__FUNCTION__) + " failed to open /sys/block");
    }

    std::regex regexp("sd([A-Za-z]|[A-Za-z][0-9])+");

    // get first directory
    auto nextDir = readdir(m_dirDisks.get());

    while(nextDir != nullptr) {
        if(std::regex_match(nextDir->d_name, regexp)) {
            // get directories subdirectories
            std::string&& path = std::string(SYSFS_DIRECTORY) + nextDir->d_name;

            auto subDir = opendir(path.c_str());

            std::vector<PartitionInfo> subdirs;

            auto nextSubDir = readdir(subDir);

            while(nextSubDir != nullptr) {
                if(std::regex_match(nextSubDir->d_name, regexp)) {
                    // open stat file
                    // add to list
                    PartitionInfo partition = {
                      nextSubDir->d_name,
                      fopen(getFullPath(nextSubDir->d_name + std::string("stat")).c_str(),
                                        std::string("r").c_str())
                    };
                    subdirs.push_back(std::move(partition));
                }
                nextSubDir = readdir(subDir);
            }
            m_disksAndPartions.insert({ nextDir->d_name, std::move(subdirs) });
        }
        nextDir = readdir(m_dirDisks.get());
    }
}


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
std::string DiskInformation::getFullPath(const std::string& diskName)
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





