#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cstdint>

#include "headers/argument_processor.hpp"
#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

#define MIN_ARGUMENTS 6

int32_t ArgumentProcessor::openPcapFile()
{
    try 
    {
        inputFile.open(optarg);
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "Failed to open log file: " << e.what() << std::endl;
        return SYSTEM_ERR;
    }
    return SUCCESS;
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
    if (argc < MIN_ARGUMENTS)
    {
        this->printHelp();
        return INVALID_CMDL_OPTIONS;
    }

    int32_t opt;
    while ((opt = getopt(argc, argv, "i:r:")) != -1)
    {
        switch (opt)
        {
            case 'r':
            {
                if (this->openPcapFile() == SYSTEM_ERR)
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
            delete parser;
            return -1;
        }
    }
    delete parser;
    return SUCCESS;
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

std::string ArgumentProcessor::getInterface()
{
    return interface;
}