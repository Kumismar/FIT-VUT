#pragma once

#include <string>

class IpAddressParser 
{
    private:
        std::string token;

        int32_t parseMask();
        int32_t parseByte();
    public:
        int32_t parseIPAddress(std::string& ipAddr);
};