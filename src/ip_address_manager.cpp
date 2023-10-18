#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <cmath>
#include <ncurses.h>

#include "headers/ip_address_manager.hpp"
#include "headers/constants.h"

int32_t IpAddressManager::setAddressesAndMasks(std::shared_ptr<std::vector<std::string>> addresses)
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
    this->networkUtilizations.push_back(NO_UTILIZATION);
}

void IpAddressManager::addMaskToArray(std::string& mask)
{
    std::stringstream auxMask(mask);
    uint32_t decimalMask;
    auxMask >> decimalMask;
    uint32_t totalAddressesAvailable = std::pow(2, MAX_MASK_NUMBER - decimalMask) - 2;

    this->decimalMasks.push_back(decimalMask);
    this->numberOfTakenAddresses.push_back(NO_ADDRESSES_TAKEN);
    this->maxHosts.push_back(totalAddressesAvailable);
}

void IpAddressManager::printMembers()
{
    clear();
    printw("%-15s %-12s %-20s %-10s\n", "IP-Prefix", "Max-Hosts", "Allocated addresses", "Utilization");
    for (size_t i = 0; i < this->networkAddresses.size(); i++)
    {
        struct in_addr tmp;
        tmp.s_addr = (in_addr_t)(this->networkAddresses[i]);
        inet_aton(this->charAddress, &tmp);
        char ipPrefixForPrint[INET_ADDRSTRLEN + 3];
        std::strcpy(ipPrefixForPrint, this->charAddress);
        std::strcat(ipPrefixForPrint, ('/' + std::to_string(this->decimalMasks[i])).c_str());
        printw("%-15s %-12d %-20d %.2f%%\n",
             ipPrefixForPrint, this->maxHosts[i], this->numberOfTakenAddresses[i], this->networkUtilizations[i]
        );
    }
    refresh();
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
            double networkRange = std::pow(2, MAX_MASK_NUMBER - this->decimalMasks[i]);
            float maxIpAddressesInNetwork = (float)(networkRange - NETWORK_AND_BROADCAST);
            float utilization = (float)(this->numberOfTakenAddresses[i]) / maxIpAddressesInNetwork * CONVERT_TO_PERCENT;

            this->takenAddresses[i].push_back(clientAddress);
            this->networkUtilizations[i] = utilization;
        }
    }
}

bool IpAddressManager::belongsToNetwork(uint32_t clientAddressShifted, uint32_t networkAddressShifted)
{
    return (clientAddressShifted & networkAddressShifted) == networkAddressShifted;
}

bool IpAddressManager::isTaken(uint32_t clientAddress, size_t index)
{
    if (this->takenAddresses.size() < index || this->takenAddresses.empty())
    {
        std::vector<uint32_t> tmp;
        tmp.push_back(clientAddress);
        this->takenAddresses.push_back(tmp);
        return false;
    }

    if (this->takenAddresses[index].empty())
    {
        this->takenAddresses[index].push_back(clientAddress);
        return false;
    }

    for (uint32_t takenAddr : takenAddresses[index])
    {
        if (takenAddr == clientAddress)
            return true;
    }

    return false;
}

