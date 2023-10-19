#pragma once

#include <pcap.h>
#include <fstream>
#include <memory>
#include <cstdint>
#include "argument_processor.hpp"
#include "ip_address_manager.hpp"
/**
 * @brief Class responsible for sniffing and extracting information from packets.
 */
class PacketSniffer
{
private:
    /** C-string style name of interface that the program listens on. */
    char* interface = nullptr;

    /** C-string style name of file that the program will read packets from. */
    char* inputFileName = nullptr;

    /** Sniffing session handler. */
    pcap_t* handle;

    /** Structure representing instructions processing packets. */
    struct bpf_program filterProgram;

    /** Header of captured packets. */
    struct pcap_pkthdr* packetHeader;

    /** Data of captured packets. */
    const u_char* packetData;

    /**
     * @brief Extracts IP addresses from DHCP packets and calls IP Address Manmager to process it.
     * @param manager IP Address Manager instance.
     */
    void processPacket(std::shared_ptr<IpAddressManager> manager);

public:
    /**
     * @brief Frees allocated memory for c-style string fields.
     */
    ~PacketSniffer();

    /**
     * @brief Setter for this->inputFileName.
     * @param fileName File in which the program reads packets.
     */
    void setInputFile(char* fileName);

    /**
     * @brief Setter for this->interface.
     * @param dev Interface on which the program listens for packets.
     */
    void setInterface(char* dev);

    /**
     * @brief Sets up sniffing session handler, compiles filter and prepares filtering program instructions.
     * @return SUCCESS (0) if everything runs smoothly
     * @return FAIL (-1) when an error occurs
     */
    int32_t setUpSniffing();

    /**
     * @brief Sniffs for packets in a loop, calling sufficient methods for processing them.
     * @param addresses Addresses given to program as command-line arguments,
     * are further passed to IP Address Manager.
     * @return SUCCESS (0) when a packet has been read successfully
     * @return FAIL (-1) on error when reading packet.
     */
    int32_t sniffPackets(std::vector<std::string>& addresses);

    /**
     * @brief Ends handling session and frees memory allocated for filtering program instructions.
     */
    void cleanUp();
};
