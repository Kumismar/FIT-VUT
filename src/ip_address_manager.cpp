#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "headers/ip_address_manager.hpp"
#include "headers/errors.h"

#define MAX_MASK_NUMBER 32
#define NO_ADDRESSES_TAKEN 0

int32_t IpAddressManager::setAddressesAndMasks(const std::shared_ptr<std::vector<std::string>>& addresses)
{
    for (std::string& ipAddress : *addresses)
    {
        size_t separatorPosition = ipAddress.find('/');
        std::string address = ipAddress.substr(0, separatorPosition);
        std::string mask = ipAddress.substr(separatorPosition + 1, std::string::npos);
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
    this->networkAddresses.push_back(binaryIpAddr);
}

void IpAddressManager::addMaskToArray(std::string& mask)
{
    std::stringstream auxMask(mask);
    uint32_t decimalMask;
    auxMask >> decimalMask;
    uint32_t binaryMask = 0xFFFFFFFF >> (MAX_MASK_NUMBER - decimalMask);
    uint32_t totalAddressesAvailable = std::pow(2, MAX_MASK_NUMBER - decimalMask) - 2;

    this->decimalMasks.push_back(decimalMask);
    this->numberOfFreeAddresses.push_back(totalAddressesAvailable);
    this->numberOfTakenAddresses.push_back(NO_ADDRESSES_TAKEN);
}

void IpAddressManager::printMembers()
{
    for (size_t i = 0; i < networkAddresses.size(); i++)
    {
        struct in_addr tmp;
        tmp.s_addr = (in_addr_t)networkAddresses[i];
        struct in_addr tmp2;
        std::cout << "ip address: " << inet_ntoa(tmp) << "\tmask: " << inet_ntoa(tmp2) << "\tdecimal mask: " << decimalMasks[i]
                  << "\ttaken addresses: " << numberOfTakenAddresses[i] << "\tfree addresses:" << numberOfFreeAddresses[i] << std::endl;
    }
}

void IpAddressManager::processNewAddress(struct in_addr& addr)
{
    for (size_t i = 0; i < this->networkAddresses.size(); i++)
    {
        uint32_t networkAddress = this->networkAddresses[i];
        uint32_t clientAddress = (uint32_t)addr.s_addr;
        uint32_t numOfShifts = MAX_MASK_NUMBER - this->decimalMasks[i];
        uint32_t networkAddressShifted = networkAddress << numOfShifts;
        uint32_t clientAddressShifted = clientAddress << numOfShifts;
        if (this->belongsToNetwork(clientAddressShifted, networkAddressShifted) && !this->isTaken(clientAddress, i))
        {
            this->numberOfTakenAddresses[i]++;
            this->numberOfFreeAddresses[i]--;
            this->takenAddresses[i].push_back(clientAddress);
        }
    }
}

bool IpAddressManager::belongsToNetwork(uint32_t clientAddressShifted, uint32_t networkAddressShifted)
{
    return (clientAddressShifted & networkAddressShifted) == networkAddressShifted;
}

bool IpAddressManager::isTaken(uint32_t clientAddress, size_t index)
{
    if (this->takenAddresses.size() < index)
    {
        std::vector<uint32_t> tmp;
        tmp.push_back(clientAddress);
        this->takenAddresses.push_back(tmp);
        return false;
    }

    for (uint32_t takenAddr : takenAddresses[index])
    {
        if (clientAddress == takenAddr)
            return true;
    }
    return false;
}

