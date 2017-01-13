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

#ifndef CALL_PRORAM_H
#define CALL_PRORAM_H

#include "iinfoobject.h"

class  CallProgram: public IInfoObject {
public:
    void initialize(){}
    void update(){}

    static std::string execute(const char* command){
        std::string output;
        char buffer[100];

        FILE* pipe = popen(command, "r");

        if(!pipe){
            output = "";
        }
        else{
            while (!feof(pipe)) {
                if(fgets(buffer, sizeof(buffer), pipe) != nullptr)
                    output += buffer;
            }
        }

        pclose(pipe);

        return output;
    }
};

#endif // CALL_PRORAM_H
