#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

#include "headers/ip_address_manager.hpp"
#include "headers/errors.h"

#define MAX_MASK_NUMBER 32

int32_t IpAddressManager::setAddressesAndMasks(std::shared_ptr<std::vector<std::string>> addresses)
{
    for (std::string& ipAddress : *addresses)
    {
        size_t separatorPosition = ipAddress.find('/');
        std::string address = ipAddress.substr(0, separatorPosition);
        ipAddress.erase(0, separatorPosition + 1);
        std::string mask = ipAddress;
        this->addAddressToArray(address);
        this->addMaskToArray(mask);
    }
    return SUCCESS;
}

void IpAddressManager::addAddressToArray(std::string& address)
{
    std::strcpy(this->charAddress, address.c_str());
    struct in_addr tmpAddr;
    inet_aton(this->charAddress, &tmpAddr);
    uint32_t binaryIpAddr = (uint32_t)tmpAddr.s_addr;
    this->ipAddresses.push_back(binaryIpAddr);
}

void IpAddressManager::addMaskToArray(std::string& mask)
{
    std::stringstream auxMask(mask);
    uint32_t maskNum;
    auxMask >> maskNum;
    uint32_t binaryMask = 0xFFFFFFFF >> (MAX_MASK_NUMBER - maskNum);
    this->ipAddressMasks.push_back(binaryMask);
}
