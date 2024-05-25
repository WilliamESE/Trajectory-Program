#pragma once

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace sparkyLib{

typedef enum {
    READ,
    WRITE
}t_filemode;

void testLogger();

class Logger {
    std::stringstream filename;
    FILE *ofile;
    int fmode;
    bool fileOpen;

    public:
        Logger(std::string name, std::string ext = "txt", t_filemode mode = WRITE);

        bool isOpen();
        void reOpen(std::string name, std::string ext = "txt", t_filemode mode = WRITE);
        std::int32_t writeline(std::string str);
        std::string readline();
        void close();
        void save();
        bool isEOF();
};
} //sparkyLib