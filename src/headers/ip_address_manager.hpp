/**
 * @file ip_address_manager.hpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "network_data.hpp"
#include "list_insertable.hpp"

class IpAddressManager : public ListInsertable
{
private:
    /** Array of network data structs that hold information about each network. */
    std::vector<NetworkData> networks;

    /**
     * @brief Sets IP address for newly created network.
     *
     * Takes last network added to this->networks.
     *
     * @param address Address to be set.
     */
    void setNetworkAddress(std::string& address);

    /**
     * @brief Sets mask and other related fields to default for newly created network.
     *
     * Related fields are numberOfTakenAddresses and maxClients, which can be calculated directly from mask number.
     *
     * @param mask Mask to be set.
     */
    void setNetworkMask(std::string& mask);

    /**
     * @brief Decides whether adress is already taken in network.
     * @param clientAddress Address to be checked.
     * @param network Network in which the clientAddress will be searched for.
     * @return true if the address is already taken.
     * @return false if the address hasn't been taken yet.
     */
    bool isTaken(uint32_t clientAddress, NetworkData& network);

    /**
     * @brief Decides whether client new address belongs to given network.
     *
     * Both of the addresses are shifted right by (32 - Mask), which takes out all the variable bits
     * in an IP address, because all the bits up to the position of Mask are the same.
     * Also checks if clientAddress is broadcast or network address.
     *
     * @param clientAddress Address to be checked.
     * @param network Network in which the clientAddress will be searched for.
     * @return
     */
    bool belongsToNetwork(uint32_t clientAddressShifted, NetworkData& network);

    /**
     * @brief Prints information to syslog if network utilization exceeds 50%
     * @param network Network that exceeded 50% utilization.
     */
    void logUtilization(NetworkData &network);

    /**
     * @brief Calculates utilization, converts it to percent and sets it to given network.
     *
     * Based on max hosts and number of currently taken addresses.
     *
     * @param network Network in which the utilization will be set.
     */
    void updateUtilization(NetworkData &network);

    /**
     * @brief Finds network broadcast address and sets it in the last added network.
     */
    void setBroadcastAddress();

public:

    /**
     * Inserts new object to AllocList.
     */
    IpAddressManager();

    /**
     * @brief Prints necessary information about networks.
     *
     * Uses ncurses library for printing the information once and just changing the values on lines.
     */
    void printMembers();

    /**
     * Parses given prefixes and creates data structs for keeping information about them.
     *
     * @param addresses prefixes given to program as command-line arguments.
     * @return
     */
    void createNetworkData(std::vector<std::string>& addresses);

    /**
     * @brief Processes new client address from sniffed DHCPACK packet.
     *
     * That includes checking if it's taken, to what networks it belongs, etc.
     *
     * @param clientAddress Client address from DHCPACK packet.
     */
    void processNewAddress(struct in_addr& clientAddress);

    /**
     * @brief Removes IP address yielded by the client in DHCPRELEASE packet and deletes all the data associated with it.
     *
     * @param clientAddress Client address from DHCPRELEASE packet.
     */
    void removeUsedIpAddr(struct in_addr& clientAddress);

    /**
     * @brief Get information of all the networks.
     * Used for unit tests only.
     * 
     * @return std::vector<NetworkData> 
     */
    std::vector<NetworkData> getData();
};


