#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cstdint>
#include <cstring>

#include "headers/argument_processor.hpp"
#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

#define MIN_ARGUMENTS 4

ArgumentProcessor::~ArgumentProcessor()
{
    if (this->interface != nullptr)
    {
        delete[] this->interface;
    }
}

int32_t ArgumentProcessor::openPcapFile()
{
    try 
    {
        this->inputFile.open(optarg);
    }
    catch (const std::ios_base::failure& e)
    {
        if (this->inputFile.bad())
        {
            std::cerr << "Failed to open input file: " << e.what() << std::endl;
            return SYSTEM_ERR;
        }
        if (this->inputFile.fail())
        {
            std::cerr << "Input file doesn't exist, you haven't got permissions to open it or something similar." << std::endl;
            return INVALID_CMDL_OPTIONS;
        }
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
        std::cerr << "Not enough arguments!" << std::endl;
        return INVALID_CMDL_OPTIONS;
    }

    int32_t opt;
    while ((opt = getopt(argc, argv, "i:r:")) != -1)
    {
        switch (opt)
        {
            case 'r':
            {
                int32_t retCode = this->openPcapFile();
                if (retCode != SUCCESS)
                {
                    return retCode;
                }
                break;
            }
            case 'i':
            {
                this->interface = new char[std::strlen(optarg) + 1];
                std::strcpy(this->interface, optarg);
                break;
            }
            default:
            {
                std::cerr << "Invalid command-line option!" << std::endl;
                return INVALID_CMDL_OPTIONS;
            }
        }
    }

    if (!this->inputFile.is_open() && this->interface == nullptr)
    {
        std::cerr << "Neither interface to listen on nor offline pcap files were provided." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }

    IpAddressParser* parser = new IpAddressParser();
    for (uint16_t i = optind; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (parser->parseIPAddress(arg) == SUCCESS)
        {
            this->ipPrefixes.push_back(arg);
        }
        else 
        {
            delete parser;
            return INVALID_CMDL_OPTIONS;
        }
    }

    delete parser;
    return SUCCESS;
}

void ArgumentProcessor::printMembers()
{
    std::cout << this->interface << std::endl;
    for (uint16_t i = 0; i < this->ipPrefixes.size(); i++)
        std::cout << this->ipPrefixes[i] << "\t";
    std::cout << std::endl; 
}

void ArgumentProcessor::closeFiles()
{
    try
    {
        this->inputFile.close();
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "Failed to close file:" << e.what() << std::endl;
    }   
}

char* ArgumentProcessor::getInterface()
{
    return this->interface;
}