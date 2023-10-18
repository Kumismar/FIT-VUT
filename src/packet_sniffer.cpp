#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <ncurses.h>

#include "headers/packet_sniffer.hpp"
#include "headers/constants.h"

PacketSniffer::~PacketSniffer()
{
    delete[] this->interface;
    delete[] this->inputFileName;
}

int32_t PacketSniffer::sniffPackets(std::vector<std::string>& addresses)
{
    std::shared_ptr<IpAddressManager> manager = std::make_shared<IpAddressManager>();
    manager->setAddressesAndMasks(addresses);
    while (true)
    {
        int32_t retCode = pcap_next_ex(this->handle, &this->packetHeader, &this->packetData);
        if (retCode == PCAP_ERROR_BREAK)
        {
            return SUCCESS;
        }
        else if (retCode == PCAP_ERROR)
        {
            std::cerr << pcap_geterr(this->handle) << std::endl;
            return FAIL;
        }
        else if (retCode == PACKET_SUCCESSFULLY_READ)
        {
            if (this->packetHeader->caplen < DHCP_TYPE_LOCATION)
            {
                std::cerr << "Can't read DHCP data; packet is too short." << std::endl;
                return FAIL;
            }
            this->processPacket(manager);
        }
        else if (retCode == CAPTURE_TIMEOUT)
        {
            std::cerr << "Live capture buffer timeout" << std::endl;
            return FAIL;
        }
        else
        {
            std::cerr << pcap_geterr(this->handle) << std::endl;
            return FAIL;
        }
    }
}

void PacketSniffer::processPacket(std::shared_ptr<IpAddressManager> manager)
{
    struct in_addr clientNewAddress;
    char clientNewAddressStr[INET_ADDRSTRLEN];
    // Skip ethernet, ip and udp headers before actual DHCP data
    struct ip* ipHeader = (struct ip*)(this->packetData + ETHER_HDR_LEN);
    const u_char* dhcpData = this->packetData + ETHER_HDR_LEN + ipHeader->ip_hl*BYTES_PER_WORD + sizeof(struct udphdr);

    if (dhcpData[MESSAGE_TYPE_LOCATION] == DHCP && dhcpData[DHCP_TYPE_LOCATION] == ACK)
    {
        memcpy(&clientNewAddress, dhcpData + CLIENT_IPADDR_POSITION, sizeof(struct in_addr));
        inet_ntop(AF_INET, &clientNewAddress, clientNewAddressStr, INET_ADDRSTRLEN);
        manager->processNewAddress(clientNewAddress);
        manager->printMembers();
    }
}

void PacketSniffer::setInputFile(char* fileName)
{
    if (fileName != nullptr) 
    {
        this->inputFileName = new char[std::strlen(fileName) + 1];
        std::strcpy(this->inputFileName, fileName);
    }
}

void PacketSniffer::setInterface(char* dev)
{
    if (dev != nullptr)
    {
        this->interface = new char[std::strlen(dev) + 1];
        std::strcpy(this->interface, dev);
    }
}

int32_t PacketSniffer::setUpSniffing()
{
    if (this->interface != nullptr && this->inputFileName == nullptr)
    {
        this->handle = pcap_open_live(this->interface, BUFSIZ, PROMISC, TIMEOUT_MS, this->pcapErrBuff);
    }
    else
    {
        this->handle = pcap_open_offline(this->inputFileName, this->pcapErrBuff);
    }

    if (this->handle == nullptr)
    {
        std::cerr << "Can't listen on interface: " << this->interface << ", additional info: " << this->pcapErrBuff << std::endl;
        return FAIL;
    }

    if (pcap_compile(this->handle, &this->filterProgram, this->filter, NO_OPTIMIZATION, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR)
    {
        std::cerr << "pcap_compile() error:\n" << pcap_geterr(this->handle) << std::endl;
        return FAIL;
    }
    if (pcap_setfilter(this->handle, &this->filterProgram) == PCAP_ERROR)
    {
        std::cerr << "pcap_setfilter() error:\n" << pcap_geterr(this->handle) << std::endl;
        return FAIL;
    }
    return SUCCESS;
}


