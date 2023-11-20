/**
 * @file ip_address_parser.cpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#include <iostream>
#include <sstream>
#include <cstdint>

#include "headers/ip_address_parser.hpp"
#include "headers/constants.h"
#include "headers/alloc_list.hpp"

IpAddressParser::IpAddressParser()
{
    AllocList.push_back(this);
}

int32_t IpAddressParser::parseIPAddress(std::string ipAddr)
{
    size_t position;
    uint8_t byteCount = 0;

    // Takes 3 bytes of IP address, leaves the last byte and mask
    while ((position = ipAddr.find('.')) != std::string::npos)
    {
        this->token = ipAddr.substr(STRING_START, position);
        int32_t retCode = this->parseByte();
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        ipAddr.erase(STRING_START, position + DELIMITER);
        byteCount++;
    }
    
    // Last byte and mask handling
    if ((position = ipAddr.find('/')) != std::string::npos)
    {
        this->token = ipAddr.substr(STRING_START, position);
        int32_t retCode = this->parseByte();
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        byteCount++;
        ipAddr.erase(STRING_START, position + DELIMITER);

        // Only "/mask" is left in the string
        this->token = ipAddr;
        retCode = this->parseMask();
        if (retCode != SUCCESS)
        {
            return retCode;
        }
    }
    else
    {
        std::cerr << "Didn't find bytes and mask separator ('/')." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }

    if (byteCount != CORRECT_IP_ADDR_BYTE_COUNT)
    {
        std::cerr << "Incorrect number of IP address bytes." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }
    return SUCCESS;
}

int32_t IpAddressParser::parseByte()
{
    std::stringstream addr(this->token);
    int32_t byte;

    try
    {
        addr >> byte;
    }
    catch (const std::ios_base::failure& e)
    {
        // Internal stringstream or operator>> error
        if (addr.bad())
        {
            std::cerr << "Internal operator>> error." << std::endl; 
            return SYSTEM_ERR;
        }
        else if (addr.fail())
        {
            std::cerr << "Couldn't extract number from IP address byte." << std::endl;
            return INVALID_CMDL_OPTIONS;
        }
    }
    
    if (byte < MIN_BYTE_VALUE || byte > MAX_BYTE_VALUE)
    {
        std::cerr << "Byte value out of range." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }
    return SUCCESS;
}

int32_t IpAddressParser::parseMask()
{
    std::stringstream mask(this->token);
    int32_t maskNum;

    try
    {
        mask >> maskNum;
    }
    catch (const std::ios_base::failure& e)
    {
        // Internal stringstream or operator>> error
        if (mask.bad())
        {
            std::cerr << "Internal operator>> error." << std::endl;
            return SYSTEM_ERR;
        }
        else if (mask.fail())
        {
            std::cerr << "Couldn't extract number from IP address mask." << std::endl;
            return INVALID_CMDL_OPTIONS;
        }
    }

    if (maskNum < MIN_MASK_NUMBER || maskNum > MAX_MASK_NUMBER)
    {
        std::cerr << "Mask number out of range." << std::endl;
        return INVALID_CMDL_OPTIONS;
    }
    return SUCCESS;
}

