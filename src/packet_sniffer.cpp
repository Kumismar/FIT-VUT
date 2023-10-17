#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"

#define NO_OPTIMIZATION 0
#define PROMISC 1
#define CLIENT_IPADDR_POSITION 16
#define MESSAGE_TYPE_LOCATION 240
#define DHCP 53
#define DHCP_TYPE_LOCATION 242
#define ACK 5
#define TIMEOUT_MS 10000
#define BYTES_PER_WORD 4
#define NUM_OF_PACKETS 10

PacketSniffer::~PacketSniffer()
{
    delete[] this->interface;
    delete[] this->inputFileName;
}

void PacketSniffer::sniffPackets()
{   
    while (true)
    {
        this->packetData = pcap_next(this->handle, this->packetHeader);

        if (this->packetHeader->caplen < DHCP_TYPE_LOCATION) 
        {
            // Pozdeji by chtelo vyresit nejaky error handling, tady tenhle pripad by asi nemel nastat
            continue;
        }
        this->processPacket();
    }
}

void PacketSniffer::processPacket()
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


