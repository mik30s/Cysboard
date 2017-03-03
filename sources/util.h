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


// MACROs and functions for handling common or repetitive tasks
#pragma once

#include <sciter/sciter-x-window.hpp>
#include <experimental/string_view>
#include <regex>

using string_view = std::experimental::string_view;

template<typename T>
struct NodeTextUpdater{
    T& supplementary;
    int counter;
    NodeTextUpdater(const T& ref):counter(0), supplementary(ref){}
    void operator()(sciter::dom::element& node){
        counter++;
        if(node.is_valid()){
            numToDomText(supplementary[counter], node);
        }
    }
};

/**
 * @brief pipeDeleter struct
 * Closes a file stream
 */
struct pipeDeleter {
    void operator()(FILE* ptr) const noexcept{
        pclose(ptr);
    }
};

/**
 * @brief Uses regexp to get the integer value of the Nth number in a string
 * @param text A reference to a std::string
 * @param pos  Which position to look at
 * @return
 */
static inline uint64_t getNthNumber(const std::string& text, int pos){
    const static std::regex regx("(?:\\d+\\s+){" + std::to_string(pos) + "}(\\d+).*");
    std::smatch matchingStrings;

    if(std::regex_match(text, matchingStrings, regx)) {
        return (uint64_t)std::stoll(matchingStrings[1]);
    }

    return 0;
}

/**
 * @brief Converts a numeric type to and sciter DOM text
 * @param source a numeric type
 * @param destination a DOM element
 */
template<class T>
static inline void numToDomText(const T& source, sciter::dom::element& destination){
    std::string&& val = std::to_string((int)std::ceil(source));

    if(destination.is_valid()) {
        destination.set_text((const WCHAR*)aux::utf2w(val));
    }
}


/**
 * @brief Safely destroys a DOM node.
 * @param A sciter DOM node
 */
static inline void destroyDomNode(sciter::dom::element& node){
    if(node.is_valid()){
        node.destroy();
    }
}

/**
 * @brief Converts a numeric type to and sciter DOM text
 * @param source a numeric type
 * @param destination  a DOM element
 */
static inline void numToDomText(const int& source, sciter::dom::element& destination){
    std::string&& val = std::to_string(source);

    if(destination.is_valid()) {
        destination.set_text((const WCHAR*)aux::utf2w(val));
    }
}


/**
 * @brief Converts an std::string to and sciter DOM text
 * @param source
 * @param destination
 */
static inline void stringToDomText(const std::string& source, sciter::dom::element& destination) {
    if(destination.is_valid()) {
        destination.set_text((const WCHAR*)aux::utf2w(source));
    }
}


/**
 * @brief find all the elements in the DOM with the selector
 * @param root The root DOM element
 * @param selector CSS selector for the nodes
 * @param nodes a vector to hold the nodes found
 * @param overwrite Replace all elements in the vector
 */
static void findAllElements(sciter::dom::element& root,
                            const char* selector,
                            std::vector<sciter::dom::element>& nodes,
                            bool overwrite = true)
{
    // clear out previous contents
    if(overwrite){
        nodes.clear();
    }

    struct NodeCounter: public sciter::dom::callback {
        decltype(nodes)& n;
        NodeCounter(const decltype(nodes)vect):n(vect){}

        inline bool on_element(HELEMENT elem) {
            n.push_back(sciter::dom::element(elem));
            return false; // continue searching
        }
    };

    NodeCounter counter(nodes);
    root.find_all(&counter, selector);
}

/**
 * @brief Converts the value using the specified multiplier
 * @param value the value to convert
 * @param multiplier MB, KB or GB
 * @return
 */
static double convertMemory(uint64_t value, const char* multiplier){
    double retVal =  0;
    const uint64_t MEGABYTE = 1048576;
    const uint64_t KILOBYTE = 1024;

    if(std::strncmp("KB", multiplier, 2) == 0){
        return value;
    }
    if(std::strncmp("MB", multiplier, 2) == 0){
        retVal = (double) value / KILOBYTE;
    }
    if(std::strncmp("GB", multiplier, 2) == 0){
        retVal = (double) value / MEGABYTE;
    }

    return std::floor(retVal * 100) / 100;
}


#define DOM_TEXT_TO_NUM(source, destination) \
            destination = aux::wtoi(source.c_str()) \

#define DOM_TEXT_TO_STR(source, destination)\
            destination = aux::w2utf(source.c_str()) \

#define DOM_TEXT_TO_CSTR(source)\
            aux::w2utf(source.c_str()).c_str()\


