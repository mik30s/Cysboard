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

#ifndef UTIL_H
#define UTIL_H

#include <sciter/sciter-x-window.hpp>
#include <experimental/string_view>
#include <regex>

using string_view = std::experimental::string_view;

/**
 * @brief Uses regexp to get the integer value of the Nth number in a string
 * @param text
 * @param pos
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
static inline void num2DomText(const T& source, sciter::dom::element& destination){
    std::string&& val = std::to_string((int)std::ceil(source));

    if(destination.is_valid()) {
        destination.set_text((const WCHAR*)aux::utf2w(val));
    }
}


/**
 * @brief Converts a numeric type to and sciter DOM text
 * @param source a numeric type
 * @param destination  a DOM element
 */
static inline void num2DomText(const int& source, sciter::dom::element& destination){
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
static inline void string2DomText(const std::string& source, sciter::dom::element& destination) {
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
static void findAllElements(sciter::dom::element& root, const char* selector,
                            std::vector<sciter::dom::element>& nodes, bool overwrite = true)
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

#define DOM_TEXT_TO_NUM(source, destination) \
            destination = aux::wtoi(source.c_str()) \

#define DOM_TEXT_TO_STR(source, destination)\
            destination = aux::w2utf(source.c_str()) \

#define DOM_TEXT_TO_CSTR(source)\
            aux::w2utf(source.c_str()).c_str()\



#endif // UTIL_H
