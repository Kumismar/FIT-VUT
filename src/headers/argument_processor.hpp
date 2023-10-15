#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <memory>

class ArgumentProcessor
{
    private:
        char* interface = nullptr;
        std::vector<std::string> ipPrefixes;
        std::shared_ptr<std::ifstream> inputFile = nullptr;

        int32_t openPcapFile();
        
    public:  
        ~ArgumentProcessor();
        char* getInterface();
        std::shared_ptr<std::ifstream> getInputFile();
        int32_t processArguments(int argc, char** argv);
        void printHelp();
        void printMembers();
        void closeFiles();
};