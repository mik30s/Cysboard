#!/bin/bash

if [ "$1" == "once" ]; then
	../packfolder.bin ../resources ../default_theme.cpp
else
	when-changed default.htm -c ../packfolder.bin ../resources ../default_theme.cpp
fi
