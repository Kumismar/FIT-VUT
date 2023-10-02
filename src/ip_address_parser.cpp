#include <iostream>
#include <sstream>
#include <cstdint>

#include "headers/ip_address_parser.hpp"
#include "headers/errors.h"

int32_t IpAddressParser::isIpAddress(std::string& ipAddr)
{
    std::string tmpIpAddr = ipAddr;
    std::string token;
    size_t position;
    uint8_t byteCount = 0;

    // Takes 3 bytes of IP address, leaves the last byte and mask
    while ((position = tmpIpAddr.find('.')) != std::string::npos)
    {
        token = tmpIpAddr.substr(0, position);
        int32_t retCode = this->isValidByte(token);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        tmpIpAddr.erase(0, position + 1);
        byteCount++;
    }
    
    // Last byte and mask handling
    if ((position = tmpIpAddr.find('/')) != std::string::npos)
    {
        token = tmpIpAddr.substr(0, position);
        int32_t retCode = this->isValidByte(token);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
        byteCount++;
        tmpIpAddr.erase(0, position + 1);
        retCode = this->isValidMask(tmpIpAddr);
        if (retCode != SUCCESS)
        {
            return retCode;
        }
    }
    return (byteCount == 4) ? SUCCESS : FAIL;
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
    
    return (byte >= 0 && byte <= UINT8_MAX) ? SUCCESS : FAIL;
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
        if (mask.bad())
        {
            return SYSTEM_ERR;
        }
        else if (mask.fail())
        {
            return FAIL;
        }
    }

    return (maskNum >= 0 && maskNum <= 32) ? SUCCESS : FAIL;
}