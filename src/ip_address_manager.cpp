#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <cmath>
#include <ncurses.h>
#include <syslog.h>

#include "headers/ip_address_manager.hpp"
#include "headers/constants.h"
#include "headers/network_data.h"

void IpAddressManager::createNetworkData(std::vector<std::string>& addresses)
{
    for (std::string& ipAddress : addresses)
    {
        size_t separatorPosition = ipAddress.find('/');
        std::string address = ipAddress.substr(0, separatorPosition);
        std::string mask = ipAddress.substr(separatorPosition + 1, std::string::npos);

        NetworkData tmp;
        this->networks.push_back(tmp);
        this->setNetworkAddress(address);
        this->setNetworkMask(mask);
        this->setBroadcastAddress();
    }
}

void IpAddressManager::setNetworkAddress(std::string& address)
{
    std::strcpy(this->charAddress, address.c_str());
    struct in_addr tmpAddr;
    inet_aton(this->charAddress, &tmpAddr);

    this->networks.back().address = (uint32_t)(ntohl(tmpAddr.s_addr));
}

void IpAddressManager::setNetworkMask(std::string& mask)
{
    std::stringstream auxMask(mask);
    uint32_t decimalMask;
    auxMask >> decimalMask; // The data should be correct here - no catching exceptions
    uint32_t totalAddressesAvailable = (uint32_t)(std::pow(2, MAX_MASK_NUMBER - decimalMask) - NETWORK_AND_BROADCAST);

    NetworkData& lastNetwork = this->networks.back();
    lastNetwork.decimalMask = decimalMask;
    lastNetwork.numberOfTakenAddresses = NO_ADDRESSES_TAKEN;
    lastNetwork.maxHosts = totalAddressesAvailable;
}

void IpAddressManager::printMembers()
{
    clear();
    printw("%-15s %-12s %-20s %-10s\n", "IP-Prefix", "Max-Hosts", "Allocated addresses", "Utilization");
    printw("-------------------------------------------------------------\n");
    for (size_t i = 0; i < this->networks.size(); i++)
    {
        NetworkData& tmpNetwork = this->networks[i];
        struct in_addr tmpAddr;
        tmpAddr.s_addr = (in_addr_t)(tmpNetwork.address);
        inet_aton(this->charAddress, &tmpAddr);
        char ipPrefixForPrint[INET_ADDRSTRLEN + MASK_LENGTH];

        std::strcpy(ipPrefixForPrint, this->charAddress);
        std::strcat(ipPrefixForPrint, ('/' + std::to_string(tmpNetwork.decimalMask)).c_str());
        printw("%-15s %-12d %-20d %.2f%%\n",
             ipPrefixForPrint, tmpNetwork.maxHosts, tmpNetwork.numberOfTakenAddresses, tmpNetwork.utilization
        );
    }
    refresh();
}

void IpAddressManager::processNewAddress(struct in_addr& clientAddr)
{
    uint32_t clientAddress = (uint32_t)(ntohl(clientAddr.s_addr));
    for (size_t i = 0; i < this->networks.size(); i++)
    {
        NetworkData& currentNetwork = this->networks[i];
        if (this->belongsToNetwork(clientAddress, currentNetwork) &&
            !this->isTaken(clientAddress, currentNetwork.takenAddresses))
        {
            currentNetwork.numberOfTakenAddresses++;
            currentNetwork.takenAddresses.push_back(clientAddress);
            this->updateUtilization(currentNetwork);
        }
    }
}

bool IpAddressManager::belongsToNetwork(uint32_t clientAddress, NetworkData& network)
{
    if (clientAddress == network.address || clientAddress == network.broadcast)
    {
        return false;
    }
    uint32_t numOfShifts = MAX_MASK_NUMBER - network.decimalMask;
    uint32_t networkAddressShifted = network.address >> numOfShifts;
    uint32_t clientAddressShifted = clientAddress >> numOfShifts;
    return clientAddressShifted == networkAddressShifted;
}

bool IpAddressManager::isTaken(uint32_t clientAddress, std::vector<uint32_t>& takenAddresses)
{
    if (takenAddresses.empty())
    {
        return false;
    }

    for (const uint32_t& takenAddr: takenAddresses)
    {
        if (takenAddr == clientAddress)
            return true;
    }

    return false;
}

void IpAddressManager::logUtilization(NetworkData &network)
{
    struct in_addr tmpAddr;
    tmpAddr.s_addr = (in_addr_t)(network.address);
    char strAddr[INET_ADDRSTRLEN + MASK_LENGTH];

    std::strcpy(strAddr, inet_ntoa(tmpAddr));
    std::strcat(strAddr, ('/' + std::to_string(network.decimalMask)).c_str());
    openlog("dhcp-stats", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "prefix %s exceeded 50%% of allocations.\n", strAddr);
    std::cout << "prefix " << strAddr << " exceeded 50%% of allocations." << std::endl;
    closelog();
}

void IpAddressManager::updateUtilization(NetworkData &network)
{
    double networkRange = std::pow(2, MAX_MASK_NUMBER - network.decimalMask);
    float maxIpAddressesInNetwork = (float)(networkRange - NETWORK_AND_BROADCAST);
    network.utilization = (float)(network.numberOfTakenAddresses / maxIpAddressesInNetwork * CONVERT_TO_PERCENT);
    if (network.utilization >= HALF_NETWORK_FULL && !network.logFlag)
    {
        this->logUtilization(network);
        network.logFlag = true;
    }
    else if (network.utilization < HALF_NETWORK_FULL && network.logFlag)
    {
        network.logFlag = false;
    }
}

void IpAddressManager::removeUsedIpAddr(struct in_addr &clientAddress)
{
    uint32_t addrToRemove = (uint32_t)(ntohl(clientAddress.s_addr));
    for (size_t i = 0; i < this->networks.size(); i++)
    {
        NetworkData& currentNetwork = this->networks[i];
        for (size_t j = 0; j < currentNetwork.takenAddresses.size(); j++)
        {
            std::vector<uint32_t>& takenAddresses = currentNetwork.takenAddresses;
            if (takenAddresses[j] == addrToRemove)
            {
                takenAddresses.erase(takenAddresses.begin() + j);
                currentNetwork.numberOfTakenAddresses--;
                this->updateUtilization(currentNetwork);
                break;
            }
        }
    }
}

void IpAddressManager::setBroadcastAddress()
{
    NetworkData& lastNetwork = this->networks.back();
    uint32_t binaryMask = 0xFFFFFFFF << (MAX_MASK_NUMBER - lastNetwork.decimalMask);
    lastNetwork.broadcast = lastNetwork.address | ~binaryMask;
}

