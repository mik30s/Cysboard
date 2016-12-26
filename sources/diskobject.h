#ifndef DISKOBJECT_H
#define DISKOBJECT_H

#include <memory>
#include "iinfoobject.h"
#include "diskinfo.h"

class DiskObject : public IInfoObject
{
private:
    DiskInformation* m_ptrDiskInfo;

public:
    DiskObject();
    ~DiskObject();

    void initialize();
    void update();
};

#endif // DISKOBJECT_H
