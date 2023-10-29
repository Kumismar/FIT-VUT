#pragma once
#include <cstdint>
#include <vector>
#include "constants.h"

typedef struct networkData
{
    /** Vector of IP Addresses given to program as command-line arguments. */
    uint32_t address;

    /** Vector of masks given to program as part of IP address as command-line arguments. */
    uint32_t decimalMask;

    /** Broadcast address of network. */
    uint32_t broadcast;

    /** Numbers on index belong to this->networkAddresses[theSameIndex] and the count itself is equal to this->takenAddresses[theSameIndex].size(). */
    uint32_t numberOfTakenAddresses;

    /** Array of address arrays, each array contains addresses belonging to this->networkAddresses on the same index. */
    std::vector<uint32_t> takenAddresses;

    /** Maximum number of hosts for each IP address in this->networkAddresses. */
    uint32_t maxHosts;

    /** Auxiliary field used in a few methods, serves as string for temporary IP addresses. */
    float utilization = NO_UTILIZATION;

    /** Network utilization of each IP address in this->networkAddresses, expressed with percentage. */
    bool logFlag = false;
} NetworkData;
