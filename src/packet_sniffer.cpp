#include <cstring>
#include <iostream>

#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"


PacketSniffer::PacketSniffer(char* dev)
{
    interface = new char[std::strlen(dev) + 1];
    std::strcpy(interface, dev);
}

PacketSniffer::~PacketSniffer()
{
    delete[] interface;
}

int32_t PacketSniffer::sniffPackets()
{
    if ((handle = pcap_open_live(interface, BUFSIZ, 1, 1000, pcapErrBuff)) == nullptr)
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

    for (int i = 0; i < 100; i++)
    {
        packetData = pcap_next(handle, &packetHeader);
        std::cout << "Delka paketu " << i << ": " << packetHeader.len << "\nData paketu: " << packetData << std::endl;
    }

    pcap_close(handle);
    return SUCCESS;
}