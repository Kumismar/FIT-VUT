#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

class IpAddressManager
{
private:
    std::vector<uint32_t> ipAddresses;
    std::vector<uint32_t> ipAddressMasks;
    char charAddress[INET_ADDRSTRLEN];

    void addAddressToArray(std::string& address);
    void addMaskToArray(std::string& mask);

public:
    int32_t setAddressesAndMasks(std::shared_ptr<std::vector<std::string>> addresses);
};


