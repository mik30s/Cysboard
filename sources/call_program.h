/*Copyright 2017 Michael Osei

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

#pragma once

#define OUT_BUF_SIZE 512

class CallProgram {
public:
    void initialize(){}
    void update(){}

    static std::string execute(const char* command) {
        std::string output("");
        output.reserve(OUT_BUF_SIZE);

        struct pipeDeleter {
            void operator()(FILE* ptr) const noexcept{
                pclose(ptr);
            }
        };

        std::unique_ptr<FILE, pipeDeleter> pipe(popen(command, "r"));

        if(pipe) {
            while (!feof(pipe.get())) {
                if(fgets(const_cast<char*>(output.data()), output.capacity(), pipe.get()) != nullptr)
                    output += output.data();
            }
        }

        return output;
    }
};

