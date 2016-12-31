TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += -Wall
PKGCONFIG += gtk+-3.0

HEADERS += ../sources/*.h

SOURCES += ../sources/cpucoreobject.cpp \
             ../sources/theme.cpp \
            ../sources/main.cpp \
            ../sources/cpuobject.cpp \
            ../sources/diskobject.cpp \
            ../sources/memoryobject.cpp \
            ../sources/osobject.cpp \

unix:SOURCES += ../sources/*_linux.cpp
win32:SOURCES += ../sources/*_win32.cpp

INCLUDEPATH += ../includes

unix:CONFIG(debug, debug|release):LIBS += -L/usr/lib -ldl
unix:CONFIG(release, debug|release):LIBS += -L/usr/lib -ldl
unix:CONFIG(debug, debug|release):LIBS += -L/usr/lib -lpthread
unix:CONFIG(release, debug|release):LIBS += -L/usr/lib -lpthread


