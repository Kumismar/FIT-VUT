#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

class IpAddressManager
{
private:
    std::vector<uint32_t> networkAddresses;
    std::vector<uint32_t> decimalMasks;
    std::vector<uint32_t> numberOfTakenAddresses;
    std::vector<std::vector<uint32_t>> takenAddresses;
    std::vector<uint32_t> maxHosts;
    std::vector<float> networkUtilizations;

    char charAddress[INET_ADDRSTRLEN];

    void addAddressToArray(std::string& address);
    void addMaskToArray(std::string& mask);
    bool isTaken(uint32_t clientAddress, size_t index);
    bool belongsToNetwork(uint32_t clientAddressShifted, uint32_t networkAddressShifted);

public:
    void printMembers();
    int32_t setAddressesAndMasks(std::shared_ptr<std::vector<std::string>> addresses);
    void processNewAddress(struct in_addr& addr);
};


