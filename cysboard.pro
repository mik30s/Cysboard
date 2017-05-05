TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
QMAKE_CXXFLAGS += -Wall
PKGCONFIG += gtk+-3.0

HEADERS += sources/*.h \
    sources/call_program.h

SOURCES += sources/cpucoreobject.cpp \
           sources/main.cpp \
           sources/cpuobject.cpp \

unix:SOURCES += sources/cpuinfo_linux.cpp \
                sources/osinfo_linux.cpp \
                sources/diskinfo_linux.cpp \
                sources/meminfo_linux.cpp \

win32:SOURCES += sources/*_win32.cpp

INCLUDEPATH += includes

unix:CONFIG(debug, debug|release):LIBS += -L/usr/lib -ldl
unix:CONFIG(release, debug|release):LIBS += -L/usr/lib -ldl
unix:CONFIG(debug, debug|release):LIBS += -L/usr/lib -lpthread
unix:CONFIG(release, debug|release):LIBS += -L/usr/lib -lpthread


