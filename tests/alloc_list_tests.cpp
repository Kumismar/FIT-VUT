#include <gtest/gtest.h>

#include "../src/headers/alloc_list.hpp"
#include "../src/headers/argument_processor.hpp"
#include "../src/headers/ip_address_manager.hpp"
#include "../src/headers/ip_address_parser.hpp"
#include "../src/headers/packet_sniffer.hpp"
#include "../src/headers/list_insertable.hpp"
#include "../src/headers/network_data.hpp"

TEST(AllocListTests, AddObject)
{
    ArgumentProcessor *ap = new ArgumentProcessor();
    EXPECT_EQ(AllocList.size(), 1);
    delete ap;
    AllocList.clear();
}

TEST(AllocListTests, AddLotOfObjects)
{
    ArgumentProcessor *ap = new ArgumentProcessor();
    IpAddressManager *ip = new IpAddressManager();
    IpAddressParser *ip2 = new IpAddressParser();
    PacketSniffer *ps = new PacketSniffer();
    EXPECT_EQ(AllocList.size(), 4);
    delete ap;
    delete ip;
    delete ip2;
    delete ps;
    AllocList.clear();
}

TEST(AllocListTests, RemoveAll)
{
    ArgumentProcessor *ap = new ArgumentProcessor();
    IpAddressManager *ip = new IpAddressManager();
    IpAddressParser *ip2 = new IpAddressParser();
    PacketSniffer *ps = new PacketSniffer();
    deleteAll();
    EXPECT_EQ(AllocList.size(), 0);
    AllocList.clear();
}
