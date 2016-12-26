#ifndef IINFOOBJECT_H
#define IINFOOBJECT_H

class IInfoObject {
public:
    virtual void initialize() = 0;
    virtual void update() = 0;
};

#endif // IINFOOBJECT_H
