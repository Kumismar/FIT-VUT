#pragma once

#include <string>

class IpAddressParser 
{
    private:
        int32_t isValidByte(std::string& token);
        int32_t isValidMask(std::string& token);
    public:
        int32_t isIpAddress(std::string& ipAddr);
};