#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

class IpAddressManager
{
private:
    /** Vector of IP Addresses given to program as command-line arguments. */
    std::vector<uint32_t> networkAddresses;

    /** Vector of masks given to program as part of IP address as command-line arguments. */
    std::vector<uint32_t> decimalMasks;

    /** Numbers on index belong to this->networkAddresses[theSameIndex] and the count itself is equal to this->takenAddresses[theSameIndex].size(). */
    std::vector<uint32_t> numberOfTakenAddresses;

    /** Array of address arrays, each array contains addresses belonging to this->networkAddresses on the same index. */
    std::vector<std::vector<uint32_t>> takenAddresses;

    /** Maximum number of hosts for each IP address in this->networkAddresses. */
    std::vector<uint32_t> maxHosts;

    /** Network utilization of each IP address in this->networkAddresses, expressed with percentage. */
    std::vector<float> networkUtilizations;

    /** Auxiliary field used in a few methods, serves as string for temporary IP addresses. */
    char charAddress[INET_ADDRSTRLEN];

    /**
     * @brief Takes address and pushes it into this->networkAddresses.
     *
     * Also pushes 0 to this->networkUtilizations.
     *
     * @param address Address to be pushed.
     */
    void addAddressToArray(std::string& address);

    /**
     * @brief Takes mask and pushes it into this->decimalMasks.
     *
     * Also calculates and pushes max hosts for given mask
     * and sets takenAddresses for current address to zero.
     *
     * @param mask Mask to be pushed.
     */
    void addMaskToArray(std::string& mask);

    /**
     * @brief Decides whether adress is already taken in network.
     * @param clientAddress Address to be checked.
     * @param index Index to this->takenAddress, where clientAddress is searched.
     * @return
     */
    bool isTaken(uint32_t clientAddress, size_t index);

    /**
     * @brief Decides whether client new address belongs to given network.
     *
     * Both of the addresses are shifted right by (32 - Mask), which takes out all the variable bits
     * in an IP address, because all the bits up to the position of Mask are the same.
     *
     * @param clientAddressShifted
     * @param networkAddressShifted
     * @return
     */
    bool belongsToNetwork(uint32_t clientAddressShifted, uint32_t networkAddressShifted);

    /**
     * @brief Prints information to syslog if network utilization exceeds 50%
     * @param index Index to vectors for finding the correct data.
     */
    void logUtilization(size_t index);

    /**
     * @brief Calculates utilization and converts it to percent.
     *
     * Based on max hosts and number of currently taken addresses.
     *
     * @param i Index to vectors for finding the correct data.
     * @return Utilization in percents.
     */
    float calculateUtilization(size_t i);

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
    void setAddressesAndMasks(std::vector<std::string>& addresses);

    /**
     * @brief Processes new client address from sniffed DHCPACK packet.
     *
     * That includes checking if it's taken, to what networks it belongs, etc.
     *
     * @param addr Client address from DHCPACK packet.
     */
    void processNewAddress(struct in_addr& addr);
};


