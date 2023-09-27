#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <sstream>

#include "headers/argument_processor.hpp"

int32_t ArgumentProcessor::openLogFile()
{
    try 
    {
        logFile.open(optarg);
        if (!logFile.is_open())
        {
            throw std::ios_base::failure("Failed to open log file" + std::string(optarg));
        }
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
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

bool ArgumentProcessor::isValidByte(std::string& token)
{
    std::stringstream addr(token);
    int32_t byte;
    if (addr >> byte) 
    { 
        return (byte >= 0 && byte <= UINT8_MAX);
    }
    
    return false;
}

bool ArgumentProcessor::isValidMask(std::string& token)
{
    std::stringstream mask(token);
    int32_t maskNum;
    if (mask >> maskNum)
    {
        return (maskNum >= 0 && maskNum <= 32);
    }

    return false;
}

bool ArgumentProcessor::isIpAddress(std::string& ipAddr)
{
    std::string tmp = ipAddr;
    std::string token;
    size_t position;
    uint8_t byteCount = 0;

    // Vezme 3 byty, tedy napr 111.111.111 a zbyde trebba 111/24
    while ((position = tmp.find('.')) != std::string::npos)
    {
        token = tmp.substr(0, position);
        if (!this->isValidByte(token))
        {
            return false;
        }
        tmp.erase(0, position + 1);
        byteCount++;
    }
    
    if ((position = tmp.find('/')) != std::string::npos)
    {
        token = tmp.substr(0, position);
        if (!this->isValidByte(token))
        {
            return false;
        }
        tmp.erase(0, position + 1);
        if (!this->isValidMask(tmp))
        {
            return false;
        }
        return true;
    }
    return false;
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
                if (this->openLogFile() == -1)
                {
                    return -1;
                }
                break;
            }
            case 'i':
            {
                interface = optarg;
                break;
            }
            default:
            {
                std::cerr << "Invalid command-line option." << std::endl;
                this->printHelp();
                return -1;
            }
        }
    }

    for (uint16_t i = optind; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (this->isIpAddress(arg))
        {
            ipPrefixes.push_back(argv[i]);
        }
        else 
        {
            return -1;
        }
    }

    return 0;
}

void ArgumentProcessor::printMembers()
{
    std::cout << this->interface << std::endl;
    for (uint16_t i = 0; i < this->ipPrefixes.size(); i++)
        std::cout << ipPrefixes[i] << "\t";
    std::cout << std::endl; 
}