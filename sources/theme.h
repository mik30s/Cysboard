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
along with Cysboard.  If not, see <http://www.gnu.org/licenses/>.*/


#ifndef THEME_H
#define THEME_H

#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include <json/json.hpp>

class Theme
{
private:
    std::string fileName;
    std::ifstream file;
public:
    Theme(std::string file); // read config file as json
    ~Theme();
    void Initialize(); // initialize cysboard
    std::string createQML(); // concatenate inidividual QML files
};

#endif // THEME_H
