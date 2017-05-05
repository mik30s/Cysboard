cmake_minimum_required (VERSION 3.1)
project (Cysboard)
#version
set(Cysboard_VERSION_MAJOR 1)
set(Cysboard_VERSION_MINOR 2)
set(CMAKE_CXX_STANDARD 14)

#includes path
include_directories("includes")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

#header files
set (${HEADERS}
 sources/ccpuid.h
 sources/cpucoreobject.h
 sources/cpuinfo.h
 sources/cpuobject.h
 sources/cysboard.h
 sources/diskinfo.h
 sources/diskobject.h
 sources/iinfoobject.h
 sources/linuxutil.h
 sources/meminfo.h
 sources/memoryobject.h
 sources/osinfo.h
 sources/osobject.h
 sources/util.h
 )

 set(SOURCES
sources/cpuinfo_linux.cpp
sources/default_theme.cpp
sources/diskinfo_linux.cpp
sources/linuxutil.cpp
sources/meminfo_linux.cpp
sources/osinfo_linux.cpp
sources/main.cpp
)

# require gtk on linux
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})


add_executable(Cysboard ${SOURCES})
target_link_libraries(Cysboard pthread dl ${GTK3_LIBRARIES})
target_compile_options(Cysboard PRIVATE "-O3" )

# compile default theme
execute_process(COMMAND echo "Packing resources...")
execute_process(COMMAND deps/bin/packfolder.bin sources/resources sources/default_theme.cpp)
# add static keyword
execute_process(COMMAND sed -i "1i static" sources/default_theme.cpp)
# add sciter library to build directory
execute_process(COMMAND cp deps/bin/libsciter-gtk-64.so build/)