#include "gtest/gtest.h"
#include "../src/headers/argument_processor.hpp"
#include "../src/headers/ip_address_parser.hpp"
#include "../src/headers/errors.h"
#include <string>

TEST(IpAddressParsing, ValidIpAddresses)
{
    IpAddressParser parser;
    std::string ipAddr = "111.111.111.111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "0.0.0.0/32";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "1.2.3.4/1";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "192.0.1.12/12";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "1.3.5.7/9";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "32.42.42.42/31";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "255.0.0.0/28";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
    ipAddr = "0.0.0.0/22";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), SUCCESS);
}

TEST(IpAddressParsing, BadSeparators)
{
    IpAddressParser parser;
    std::string ipAddr = "111,111.111.111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.111.111/111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.111-111.111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111=111.111/111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.111.111.111=24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.111.111.111.24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
}

TEST(IpAddressParsing, BytesNotDivided)
{
    IpAddressParser parser;
    std::string ipAddr = "123234.123.123/22";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.111111.111/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "0.00.0/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "1010.111.123/24";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
}

TEST(IpAddressParsing, MaskNotDivided)
{
    IpAddressParser parser;
    std::string ipAddr = "123.234.123.12322";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "255.255.123.2551";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "111.233.64.255";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);    
    ipAddr = "63.234.2.51";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
}

TEST(IpAddressParsing, InvalidMaskNumbers)
{
    IpAddressParser parser;
    std::string ipAddr = "123.234.123.123/33";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "255.255.123.255/0";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "255.255.123.255/234";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
    ipAddr = "255.255.123.255/-800";
    EXPECT_EQ(parser.parseIPAddress(ipAddr), FAIL);
}