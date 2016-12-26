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
