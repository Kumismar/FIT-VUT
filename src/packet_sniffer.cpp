#include <cstring>
#include <iostream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"

PacketSniffer::~PacketSniffer()
{
    if (interface != nullptr) 
    {
        delete[] interface;
    }
}

int32_t PacketSniffer::processPacket()
{   
    struct in_addr clientNewAddress;
    char clientNewAddressStr[INET_ADDRSTRLEN];
    uint32_t i = 0;
    while (i < 1)
    {
        packetData = pcap_next(handle, &packetHeader);

        if (packetHeader.caplen < DHCP_TYPE_LOCATION) 
        {
            // Pozdeji by chtelo vyresit nejaky error handling, tady tenhle pripad by asi nemel nastat
            continue;
        }

        // Skip ethernet, ip and udp headers before actual DHCP data
        struct ip* ipHeader = (struct ip*)(packetData + ETHER_HDR_LEN);
        const uint8_t* dhcpData = packetData + ETHER_HDR_LEN + ipHeader->ip_hl*BYTES_PER_WORD + sizeof(struct udphdr);

        if (dhcpData[MESSAGE_TYPE_LOCATION] == DHCP && dhcpData[DHCP_TYPE_LOCATION] == ACK)
        {
            memcpy(&clientNewAddress, dhcpData + CLIENT_IPADDR_POSITION, sizeof(struct in_addr));
            inet_ntop(AF_INET, &clientNewAddress, clientNewAddressStr, INET_ADDRSTRLEN);
            std::cout << "DHCP packet " << ++i << ": " << "New client IP address: " << clientNewAddressStr << std::endl;
        }   
    }

    return SUCCESS;
}

void PacketSniffer::setInterface(char* dev)
{
    if (dev != nullptr)
    {
        interface = new char[std::strlen(dev) + 1];
        std::strcpy(interface, dev);
    }
}

int32_t PacketSniffer::sniffPackets()
{
    if ((handle = pcap_open_live(interface, BUFSIZ, PROMISC, TIMEOUT_MS, pcapErrBuff)) == nullptr)
    {
        std::cerr << "Can't listen on interface: " << interface << ", additional info: " << pcapErrBuff << std::endl;
        return FAIL;
    }

    if (pcap_compile(handle, &filterProgram, filter, NO_OPTIMIZATION, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR)
    {
        std::cerr << "pcap_compile() error:\n" << pcap_geterr(handle) << std::endl;
        return FAIL;
    }

    if (pcap_setfilter(handle, &filterProgram) == PCAP_ERROR)
    {
        std::cerr << "pcap_setfilter() error:\n" << pcap_geterr(handle) << std::endl;
        return FAIL;
    }

    this->processPacket();

    pcap_close(handle);
    return SUCCESS;
}


