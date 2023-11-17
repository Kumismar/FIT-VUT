#include <gtest/gtest.h>
#include <sstream>

#include "../src/headers/ip_address_manager.hpp"
#include "../src/headers/network_data.h"

class IpAddressManagerTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        iam.createNetworkData(addresses);
        networks = iam.getData();
    }
    IpAddressManager iam;
    std::vector<NetworkData> networks;
    std::vector<std::string> addresses = 
    {
        "192.168.0.0/24",
        "10.0.1.0/24"
    };
    std::vector<std::string> addressesNoMask = 
    {
        "192.168.0.0",
        "10.0.1.0"
    };
    std::vector<std::string> broadcasts =
    {
        "192.168.0.255",
        "10.0.1.255"
    };
    std::vector<std::string> masks =
    {
        "24",
        "24"
    };
    in_addr addr1;
    in_addr addr2;
};

TEST_F(IpAddressManagerTest, NetworkDataCreated)
{
    EXPECT_EQ(networks.size(), 2);
}

TEST_F(IpAddressManagerTest, CorrectIpAddresses)
{
    addr1.s_addr = htonl(networks[0].address);
    addr2.s_addr = htonl(networks[1].address);
    std::string addr1s = std::string(inet_ntoa(addr1));
    std::string addr2s = std::string(inet_ntoa(addr2));
    EXPECT_EQ(addr1s, addressesNoMask[0]);
    EXPECT_EQ(addr2s, addressesNoMask[1]);
}

TEST_F(IpAddressManagerTest, CorrectBroadcasts)
{
    addr1.s_addr = htonl(networks[0].broadcast);
    addr2.s_addr = htonl(networks[1].broadcast);
    std::string addr1s = std::string(inet_ntoa(addr1));
    std::string addr2s = std::string(inet_ntoa(addr2));
    EXPECT_EQ(addr1s, broadcasts[0]);
    EXPECT_EQ(addr2s, broadcasts[1]);
}

TEST_F(IpAddressManagerTest, CorrectMasks)
{
    std::string mask1 = std::to_string(networks[0].decimalMask);
    std::string mask2 = std::to_string(networks[1].decimalMask);
    EXPECT_EQ(mask1, masks[0]);
    EXPECT_EQ(mask2, masks[1]);
}

TEST_F(IpAddressManagerTest, CorrectMaxHosts)
{
    EXPECT_EQ(networks[0].maxHosts, 254);
    EXPECT_EQ(networks[1].maxHosts, 254);
}

TEST_F(IpAddressManagerTest, CorrectCharAddresses)
{
    EXPECT_EQ(std::string(networks[0].charAddress), addressesNoMask[0]);
    EXPECT_EQ(std::string(networks[1].charAddress), addressesNoMask[1]);
}
