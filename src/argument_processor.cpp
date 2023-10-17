#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <memory>

#include "headers/argument_processor.hpp"
#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

#define MIN_ARGUMENTS 4

ArgumentProcessor::~ArgumentProcessor()
{
    delete[] this->interface;
    delete[] this->inputFileName;
}

void ArgumentProcessor::getFileNameFromArg()
{
    this->inputFileName = new char[std::strlen(optarg) + 1];
    std::strcpy(this->inputFileName, optarg);
}

void ArgumentProcessor::getInterfaceFromArg()
{
    this->interface = new char[std::strlen(optarg) + 1];
    std::strcpy(this->interface, optarg);
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
                this->getFileNameFromArg();
                break;
            }
            case 'i':
            {
                this->getInterfaceFromArg();
                break;
            }
            default:
            {
                std::cerr << "Invalid command-line option!" << std::endl;
                return INVALID_CMDL_OPTIONS;
            }
        }
    }

    if (this->inputFileName == nullptr && this->interface == nullptr)
    {
        std::cerr << "Neither interface to listen on nor offline pcap files were provided." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }
    else if (this->inputFileName != nullptr && this->interface != nullptr)
    {
        std::cerr << "Both interface to listen from and file to read from were provided." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }

    std::unique_ptr<IpAddressParser> parser = std::make_unique<IpAddressParser>();
    for (int32_t i = optind; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (parser->parseIPAddress(arg) == SUCCESS)
        {
            this->ipPrefixes.push_back(arg);
        }
        else 
        {
            return INVALID_CMDL_OPTIONS;
        }
    }

    return SUCCESS;
}

void ArgumentProcessor::printMembers()
{
    std::cout << this->interface << std::endl;
    for (const std::string & ipPrefix : this->ipPrefixes)
        std::cout << ipPrefix << "\t";
    std::cout << std::endl; 
}


char* ArgumentProcessor::getInterface()
{
    return this->interface;
}

char* ArgumentProcessor::getFileName()
{
    return this->inputFileName;
}

std::shared_ptr<std::vector<std::string>> ArgumentProcessor::getIpPrefixes()
{
    std::shared_ptr<std::vector<std::string>> addresses = std::make_shared<std::vector<std::string>>(this->ipPrefixes);
    return addresses;
}
