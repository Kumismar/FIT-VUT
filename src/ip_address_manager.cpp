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
#include "alloc_list.hpp"

IpAddressManager::IpAddressManager()
{
    AllocList.push_back(this);
}

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
    NetworkData& network = this->networks.back();
    std::strcpy(network.charAddress, address.c_str());
    struct in_addr tmpAddr;
    inet_aton(network.charAddress, &tmpAddr);

    network.address = (uint32_t)(ntohl(tmpAddr.s_addr));
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
    lastNetwork.maxClients = totalAddressesAvailable;
}

void IpAddressManager::printMembers()
{
    clear();
    printw("%-15s %-12s %-20s %-10s\n", "IP-Prefix", "Max-Hosts", "Allocated addresses", "Utilization");
    printw("-------------------------------------------------------------\n");
    for (NetworkData& network : this->networks)
    {
        struct in_addr tmpAddr;
        tmpAddr.s_addr = (in_addr_t)(network.address);
        inet_aton(network.charAddress, &tmpAddr);
        char ipPrefixForPrint[INET_ADDRSTRLEN + MASK_LENGTH];

        std::strcpy(ipPrefixForPrint, network.charAddress);
        std::strcat(ipPrefixForPrint, ('/' + std::to_string(network.decimalMask)).c_str());
        printw("%-15s %-12d %-20d %.2f%%\n",
               ipPrefixForPrint, network.maxClients, network.numberOfTakenAddresses, network.utilization
        );
    }
    refresh();
}

void IpAddressManager::processNewAddress(struct in_addr& clientAddress)
{
    uint32_t clientAddr = (uint32_t)(ntohl(clientAddress.s_addr));
    for (NetworkData& network : this->networks)
    {
        if (this->belongsToNetwork(clientAddr, network) &&
            !this->isTaken(clientAddr, network))
        {
            network.numberOfTakenAddresses++;
            network.takenAddresses.push_back(clientAddr);
            this->updateUtilization(network);
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

bool IpAddressManager::isTaken(uint32_t clientAddress, NetworkData &network)
{
    for (std::vector<uint32_t>::iterator takenAddress = network.takenAddresses.begin(); takenAddress < network.takenAddresses.end(); takenAddress++)
    {
        if (*takenAddress == clientAddress)
        {
            return true;
        }
    }

    return false;
}

void IpAddressManager::logUtilization(NetworkData &network)
{
    char strAddr[INET_ADDRSTRLEN + MASK_LENGTH];

    std::strcpy(strAddr, network.charAddress);
    std::strcat(strAddr, ('/' + std::to_string(network.decimalMask)).c_str());
    openlog("dhcp-stats", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "prefix %s exceeded 50%% of allocations.\n", strAddr);
    closelog();
    std::cout << "prefix " << strAddr << " exceeded 50%% of allocations." << std::endl;
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
    for (NetworkData& network : this->networks)
    {
        for (auto takenAddress = network.takenAddresses.begin();
                  takenAddress < network.takenAddresses.end();
                  takenAddress++)
        {
            if (*takenAddress == addrToRemove)
            {
                network.takenAddresses.erase(takenAddress);
                network.numberOfTakenAddresses--;
                this->updateUtilization(network);
                return;
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

std::vector<NetworkData> IpAddressManager::getData()
{
    return networks;
}