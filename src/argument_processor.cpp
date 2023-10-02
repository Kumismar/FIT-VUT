#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cstdint>

#include "headers/argument_processor.hpp"
#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

int32_t ArgumentProcessor::openLogFile()
{
    try 
    {
        logFile.open(optarg);
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "Failed to open log file: " << e.what() << std::endl;
        return SYSTEM_ERR;
    }
    return 0;
}

void ArgumentProcessor::printHelp()
{
    std::cout << "Usage:\n\n" 
              << "./dhcp-stats [-r <filename>] [-i <interface-name>] <ip-prefix> [ <ip-prefix> [...] ]\n\n" 
              << "-r <filename>     Logging file\n" 
              << "-i <interface>    Interface on which the program will listen on\n" 
              << "   <ip-prefix>    Network prefix for which the statistics will be generated for" << std::endl;
}

int32_t ArgumentProcessor::processArguments(int32_t argc, char** argv)
{
    if (argc < 6)
    {
        this->printHelp();
        return -1;
    }

    int32_t opt;
    while ((opt = getopt(argc, argv, "i:r:")) != -1)
    {
        switch (opt)
        {
            case 'r':
            {
                if (this->openLogFile() == SYSTEM_ERR)
                {
                    return SYSTEM_ERR;
                }
                break;
            }
            case 'i':
            {
                interface = std::string(optarg);
                break;
            }
            default:
            {
                return INVALID_CMDL_OPTIONS;
            }
        }
    }

    IpAddressParser* parser = new IpAddressParser();
    for (uint16_t i = optind; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (parser->isIpAddress(arg))
        {
            ipPrefixes.push_back(arg);
        }
        else 
        {
            return -1;
        }
    }
    delete parser;
    return 0;
}

void ArgumentProcessor::printMembers()
{
    std::cout << this->interface << std::endl;
    for (uint16_t i = 0; i < this->ipPrefixes.size(); i++)
        std::cout << ipPrefixes[i] << "\t";
    std::cout << std::endl; 
}

void ArgumentProcessor::closeFiles()
{
    try
    {
        this->logFile.close();
        this->inputFile.close();
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "Failed to close file:" << e.what() << std::endl;
    }   
}