#pragma once

#include <string>
#include <vector>
#include <fstream>

class ArgumentProcessor
{
    private:
        char* interface = nullptr;
        std::vector<std::string> ipPrefixes;
        std::ifstream inputFile;

        int32_t openPcapFile();
        
    public:  
        ~ArgumentProcessor();
        char* getInterface();
        int32_t processArguments(int argc, char** argv);
        void printHelp();
        void printMembers();
        void closeFiles();
};