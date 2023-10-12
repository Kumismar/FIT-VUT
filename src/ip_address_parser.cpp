#include <iostream>
#include <sstream>
#include <cstdint>

#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

#define STRING_START 0
#define CORRECT_IP_ADDR_BYTE_COUNT 4
#define MAX_MASK_NUMBER 32
#define MIN_MASK_NUMBER 1
#define MIN_BYTE_VALUE 0
#define MAX_BYTE_VALUE 255

int32_t IpAddressParser::isIpAddress(std::string& ipAddr)
{
    std::string tmpIpAddr = ipAddr;
    std::string token;
    size_t position;
    uint8_t byteCount = 0;

    // Takes 3 bytes of IP address, leaves the last byte and mask
    while ((position = tmpIpAddr.find('.')) != std::string::npos)
    {
        token = tmpIpAddr.substr(STRING_START, position);
        int32_t retCode = this->isValidByte(token);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        uint8_t charsToRemove = position + 1;
        tmpIpAddr.erase(STRING_START, charsToRemove);
        byteCount++;
    }
    
    // Last byte and mask handling
    if ((position = tmpIpAddr.find('/')) != std::string::npos)
    {
        token = tmpIpAddr.substr(STRING_START, position);
        int32_t retCode = this->isValidByte(token);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        byteCount++;
        uint8_t charsToRemove = position + 1;
        tmpIpAddr.erase(STRING_START, charsToRemove);
        retCode = this->isValidMask(tmpIpAddr);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
    }
    else
    {
        return FAIL;
    }
    return (byteCount == CORRECT_IP_ADDR_BYTE_COUNT) ? SUCCESS : FAIL;
}

int32_t IpAddressParser::isValidByte(std::string& token)
{
    std::stringstream addr(token);
    int32_t byte;

    try
    {
        addr >> byte;
    }
    catch (const std::ios_base::failure& e)
    {
        // Internal stringstream error
        if (addr.bad())
        {
            return SYSTEM_ERR;
        }
        else if (addr.fail())
        {
            return FAIL;
        }
    }
    
    return (byte >= MIN_BYTE_VALUE && byte <= MAX_BYTE_VALUE) ? SUCCESS : FAIL;
}

int32_t IpAddressParser::isValidMask(std::string& token)
{
    std::stringstream mask(token);
    int32_t maskNum;

    try
    {
        mask >> maskNum;
    }
    catch (const std::ios_base::failure& e)
    {
        // Internal stringstream error
        if (mask.bad())
        {
            return SYSTEM_ERR;
        }
        else if (mask.fail())
        {
            return FAIL;
        }
    }

    return (maskNum >= MIN_MASK_NUMBER && maskNum <= MAX_MASK_NUMBER) ? SUCCESS : FAIL;
}