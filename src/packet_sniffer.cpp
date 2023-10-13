#include <cstring>
#include <iostream>

#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"


PacketSniffer::PacketSniffer(char* dev)
{
    interface = new char[std::strlen(dev) + 1];
    std::strcpy(interface, dev);
    filter = new char[std::strlen("udp port 67 or udp port 68") + 1];
    std::strcpy(filter, "udp port 67 or udp port 68");
}

PacketSniffer::~PacketSniffer()
{
    delete[] interface;
    delete[] filter;
}

int32_t PacketSniffer::processPacket()
{   
    struct in_addr clientNewAddress;
    char clientNewAddressStr[INET_ADDRSTRLEN];
    uint32_t i = 0;
    while (true)
    {
        packetData = pcap_next(handle, &packetHeader);

        if (packetHeader.caplen < DHCP_TYPE_LOCATION) 
        {
            // Pozdeji by chtelo vyresit nejaky error handling, tady tenhle pripad by asi nemel nastat
            continue;
        }

        if (packetData[MESSAGE_TYPE_LOCATION] == DHCP && packetData[DHCP_TYPE_LOCATION] == ACK)
        {
            memcpy(&clientNewAddress, packetData + CLIENT_IPADDR_POSITION, sizeof(struct in_addr));
            inet_ntop(AF_INET, &clientNewAddress, clientNewAddressStr, INET_ADDRSTRLEN);
            std::cout << "DHCP packet " << ++i << ": " << "New client IP address: " << clientNewAddressStr << std::endl;
        }   
    }
    return SUCCESS;
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


