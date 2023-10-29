#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include "network_data.h"

class IpAddressManager
{
private:
    std::vector<NetworkData> networks;

    char charAddress[INET_ADDRSTRLEN];

    /**
     * @brief Takes address and pushes it into this->networkAddresses.
     *
     * Also pushes 0 to this->networkUtilizations.
     *
     * @param address Address to be pushed.
     */
    void setNetworkAddress(std::string& address);

    /**
     * @brief Takes mask and pushes it into this->decimalMasks.
     *
     * Also calculates and pushes max hosts for given mask
     * and sets takenAddresses for current address to zero.
     *
     * @param mask Mask to be pushed.
     */
    void setNetworkMask(std::string& mask);

    /**
     * @brief Decides whether adress is already taken in network.
     * @param clientAddress Address to be checked.
     * @param index Index to this->takenAddress, where clientAddress is searched.
     * @return
     */
    bool isTaken(uint32_t clientAddress, std::vector<uint32_t>& takenAddresses);

    /**
     * @brief Decides whether client new address belongs to given network.
     *
     * Both of the addresses are shifted right by (32 - Mask), which takes out all the variable bits
     * in an IP address, because all the bits up to the position of Mask are the same.
     *
     * @param clientAddress
     * @param networkAddress
     * @return
     */
    bool belongsToNetwork(uint32_t clientAddressShifted, NetworkData& network);

    /**
     * @brief Prints information to syslog if network utilization exceeds 50%
     * @param network Index to vectors for finding the correct data.
     */
    void logUtilization(NetworkData &network);

    /**
     * @brief Calculates utilization and converts it to percent.
     *
     * Based on max hosts and number of currently taken addresses.
     *
     * @param i Index to vectors for finding the correct data.
     * @return Utilization in percents.
     */
    void updateUtilization(NetworkData &network);

    /**
     * @brief Finds network broadcast address and sets it to NetworkData.
     */
    void setBroadcastAddress();

public:

    /**
     * @brief Prints necessary information mentioned in project assignment.
     *
     * Uses ncurses library for printing the information once and just changing the values.
     */
    void printMembers();

    /**
     * Parses given addresses down to address and mask, fills in all the necessary vectors.
     *
     * The filling itself is done by methods this->add(Address/Mask)ToArray().
     * @param addresses Addresses with masks given to program as command-line arguments.
     * @return
     */
    void createNetworkData(std::vector<std::string>& addresses);

    /**
     * @brief Processes new client address from sniffed DHCPACK packet.
     *
     * That includes checking if it's taken, to what networks it belongs, etc.
     *
     * @param clientAddr Client address from DHCPACK packet.
     */
    void processNewAddress(struct in_addr& addr);
    void processNewAddress(struct in_addr& clientAddr);
};


