#include "Logger.hpp"

using namespace sparkyLib;
using namespace std;

Logger::Logger(string name, std::string ext, t_filemode mode) {
    filename << "/usd/" << name << "." << ext;
    fmode = mode;
    fileOpen = false;
    if(fmode == WRITE){
        ofile = fopen(filename.str().c_str(),"w");
        if (ofile)
            fileOpen = true;
    }
    else{
        ofile = fopen(filename.str().c_str(),"r");
        if (ofile)
            fileOpen = true;
    }
}

bool Logger::isOpen() {
    return fileOpen;
}

int32_t Logger::writeline(string str) {
    if(!fileOpen || fmode == READ)
        return 0;
    str.append("\n");
    fputs(str.c_str(), ofile);
    return 1;
}

string Logger::readline() {
    if(!fileOpen || fmode == WRITE)
        return 0;
    char line[255];
    int len = 255;
    auto read = fgets(line,len,ofile);
    if(!read){
        string empty("");
        return empty;
    }
    string str(line);
    return str;
}

bool Logger::isEOF(){
    return feof(ofile);
}

void Logger::reOpen(std::string name, std::string ext, t_filemode mode){
    filename << "/usd/" << name << "." << ext;
    fmode = mode;
    fileOpen = false;
    if(fmode == WRITE){
        ofile = fopen(filename.str().c_str(),"w");
        if (ofile)
            fileOpen = true;
    }
    else{
        ofile = fopen(filename.str().c_str(),"r");
        if (ofile)
            fileOpen = true;
    }
}

void Logger::close(){
    if(fileOpen){
        fclose(ofile);
        fileOpen = false;
    }
}

void Logger::save(){
    //No point in saving a file you are reading
    if(fmode != WRITE)
        return;
    close();
    //Reopen as append
    fileOpen = false;
    ofile = fopen(filename.str().c_str(),"a");
     if (ofile)
        fileOpen = true;
}


void sparkyLib::testLogger(){
    //Open file to write
    Logger f("testFile","txt");
    
    //Write data
    std::cout << "Writing Section 1" << std::endl;
    std::stringstream str;
    for(int x=0;x<10;x++){
        str << x << ", " << (x*0.1);
        std::cout << "Writing: " << str.str() << std::endl;
        f.writeline(str.str());
        str.str("");
    }

    //Save file
    std::cout << "Saving" << std::endl;
    f.save();

    //Write some more data
    std::cout << "Writing Section 2" << std::endl;
    for(int x=10;x<20;x++){
        str << x << ", " << (x*0.1);
        std::cout << "Writing: " << str.str() << std::endl;
        f.writeline(str.str());
        str.str("");
    }

    //Close file
    std::cout << "Writing Complete" << std::endl;
    f.close();

    //Open for read
    Logger r("testFile","txt",sparkyLib::READ);

    //Read in data
    std::cout << "Reading Data" << std::endl;
    std::string sstr;
    for(int x=0;x<20;x++){
        sstr = r.readline();
        //Print data
        std::cout << "Read: " << sstr;
    }

    //Close
    std::cout << "Test Complete" << std::endl;
    r.close();
}