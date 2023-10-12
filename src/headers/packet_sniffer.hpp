#pragma once

#include <pcap.h>

#define NO_OPTIMIZATION 0

class PacketSniffer 
{
    private:
        char pcapErrBuff[PCAP_ERRBUF_SIZE];
        char* interface;
        char* filter;
        pcap_t* handle;
        struct bpf_program filterProgram;
        struct pcap_pkthdr packetHeader;
        const uint8_t* packetData;

    public:
        PacketSniffer(char* interface);
        ~PacketSniffer();
        int32_t sniffPackets();
};