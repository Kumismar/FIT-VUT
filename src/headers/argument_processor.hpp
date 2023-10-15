#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <memory>

class ArgumentProcessor
{
    private:
        char* interface = nullptr;
        char* inputFileName = nullptr;
        std::vector<std::string> ipPrefixes;

        void getFileNameFromArg();    
        void getInterfaceFromArg();    
    public:  
        ~ArgumentProcessor();
        char* getInterface();
        char* getFileName();
        int32_t processArguments(int argc, char** argv);
        void printHelp();
        void printMembers();
        void closeFiles();
};