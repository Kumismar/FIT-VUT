#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <ncurses.h>
#include <chrono>
#include <thread>

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
    manager->createNetworkData(addresses);
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
                std::cerr << "Can't read DHCP_MESSAGE_TYPE_OPTION data; packet is too short." << std::endl;
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
    struct in_addr tmpAddress;
    char tmpAddressStr[INET_ADDRSTRLEN];

    u_char* dhcpData = this->skipToDHCPData();
    u_char* options = this->skipToOptions(dhcpData);
    u_char messageType = this->findDHCPMessageType(options);

    if (messageType != DHCPACK && messageType != DHCPRELEASE)
    {
        return;
    }
    memcpy(&tmpAddress, dhcpData + CLIENT_IPADDR_POSITION, sizeof(struct in_addr));
    inet_ntop(AF_INET, &tmpAddress, tmpAddressStr, INET_ADDRSTRLEN);
    if (messageType == DHCPACK)
    {
        manager->processNewAddress(tmpAddress);
    }
    else // DHCPRELEASE
    {
        manager->removeUsedIpAddr(tmpAddress);
    }
    manager->printMembers();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
    char pcapErrBuff[PCAP_ERRBUF_SIZE];
    char filter[] = "udp port 67";
    if (this->interface != nullptr && this->inputFileName == nullptr)
    {
        this->handle = pcap_open_live(this->interface, BUFSIZ, PROMISC, TIMEOUT_MS, pcapErrBuff);
    }
    else
    {
        this->handle = pcap_open_offline(this->inputFileName, pcapErrBuff);
    }

    if (this->handle == nullptr)
    {
        std::cerr << "Can't listen on interface: " << this->interface << ", additional info: " << pcapErrBuff << std::endl;
        return FAIL;
    }

    if (pcap_compile(this->handle, &this->filterProgram, filter, NO_OPTIMIZATION, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR)
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

void PacketSniffer::cleanUp()
{
    pcap_freecode(&this->filterProgram);
    pcap_close(this->handle);
}

u_char *PacketSniffer::skipToDHCPData()
{
    // Skip ethernet, ip and udp headers before actual DHCP_MESSAGE_TYPE_OPTION data
    struct ip* ipHeader = (struct ip*)(this->packetData + ETHER_HDR_LEN);
    return (u_char*)(this->packetData + ETHER_HDR_LEN + ipHeader->ip_hl*BYTES_PER_WORD + sizeof(struct udphdr));
}

u_char PacketSniffer::findDHCPMessageType(u_char *options)
{
    size_t i = 0;
    while(options[i] != DHCP_MESSAGE_TYPE_OPTION && options[i] != PACKET_END_OPTION)
    {
        uint32_t optionLengthLocation = i + 1;
        i += options[optionLengthLocation] + SKIP_OPTIONCODE_AND_LENGTH;
    }
    uint32_t optionData = i + 2;
    return options[optionData];
}

u_char *PacketSniffer::skipToOptions(u_char *dhcpData)
{
    return dhcpData + DHCP_OPTIONS_LOCATION;
}


