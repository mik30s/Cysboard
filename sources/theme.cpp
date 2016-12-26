#include "theme.h"

using json = nlohmann::json;

Theme::Theme(std::string fileName) {
    file.open(fileName);

    if(!file.is_open()) {
        throw std::runtime_error("Failed to open " + fileName);
    }

    std::stringstream sstream;
    sstream << file.rdbuf();
    std::string configJson; // config file contents as JSON

    configJson = sstream.str();

    json configObject = json::parse(configJson);
}

Theme::~Theme() {
    file.close();
}


void initialize(){

}


std::string createQML(){
    return "";
}

