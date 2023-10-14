#pragma once

#include <pcap.h>

#define NO_OPTIMIZATION 0
#define PROMISC 1
#define CLIENT_IPADDR_POSITION 16
#define MESSAGE_TYPE_LOCATION 240
#define DHCP 53
#define DHCP_TYPE_LOCATION 242
#define ACK 5
#define TIMEOUT_MS 10000
#define BYTES_PER_WORD 4

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

        int32_t processPacket();

    public:
        PacketSniffer(char* interface);
        ~PacketSniffer();
        int32_t sniffPackets();
};
