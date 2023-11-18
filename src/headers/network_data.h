#pragma once
#include <cstdint>
#include <vector>
#include "constants.h"
#include <arpa/inet.h>

typedef struct networkData
{
    /** IP address in binary form. */
    uint32_t address;

    /** Network mask in decimal form. */
    uint32_t decimalMask;

    /** Broadcast address in binary form. */
    uint32_t broadcast;

    /** Number of clients that have IP address allocated in network. */
    uint32_t numberOfTakenAddresses;

    /** Vector of allocated IP addresses. */
    std::vector<uint32_t> takenAddresses;

    /** Maximum number of clients that can have IP address allocated. */
    uint32_t maxClients;

    /** Utilization of network in percents. */
    float utilization = NO_UTILIZATION;

    /**
     *  A flag that tells IpAddressManager whether allocation info has already been printed. 
     *  Is reset when allocation drops below 50%.
     */
    bool logFlag = false;

    /** Network address as c-style string. */
    char charAddress[INET_ADDRSTRLEN];

} NetworkData;
