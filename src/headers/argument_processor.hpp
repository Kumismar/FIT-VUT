#pragma once

#include <string>
#include <vector>
#include <fstream>

class ArgumentProcessor
{
    private:
        std::string interface;
        std::vector<std::string> ipPrefixes;
        std::ifstream inputFile;
        std::ofstream logFile;

        int32_t openPcapFile();
        
    public:  
        std::string getInterface();
        int32_t processArguments(int argc, char** argv);
        void printHelp();
        void printMembers();
        void closeFiles();
};