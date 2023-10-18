#pragma once

#include <pcap.h>
#include <fstream>
#include <memory>
#include <cstdint>
#include "argument_processor.hpp"
#include "ip_address_manager.hpp"

class PacketSniffer
{
private:
    char pcapErrBuff[PCAP_ERRBUF_SIZE];
    char* interface = nullptr;
    char* inputFileName = nullptr;
    char filter[12] = "udp port 67";
    pcap_t* handle;
    struct bpf_program filterProgram;
    struct pcap_pkthdr* packetHeader;
    const u_char* packetData;

    void processPacket(std::shared_ptr<IpAddressManager> manager);

public:
    ~PacketSniffer();
    void setInputFile(char* fileName);
    void setInterface(char* dev);
    int32_t setUpSniffing();
    int32_t sniffPackets(std::shared_ptr<std::vector<std::string>> addresses);
};
