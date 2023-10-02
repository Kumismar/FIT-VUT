#include "gtest/gtest.h"
#include "../src/headers/argument_processor.hpp"
#include "../src/headers/ip_address_parser.hpp"
#include "../src/headers/errors.h"
#include <string>

TEST(IpAddressParsing, ValidIpAddresses)
{
    IpAddressParser parser;
    std::string ipAddr = "111.111.111.111/24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "0.0.0.0/32";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "1.2.3.4/0";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "192.0.1.12/12";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "1.3.5.7/9";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "32.42.42.42/31";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
    ipAddr = "255.0.0.0/28";
    EXPECT_EQ(parser.isIpAddress(ipAddr), SUCCESS);
}

TEST(IpAddressParsing, BadSeparators)
{
    IpAddressParser parser;
    std::string ipAddr = "111,111.111.111/24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), FAIL);
    ipAddr = "111.111.111/111/24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), FAIL);
    ipAddr = "111.111-111.111/24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), FAIL);
    ipAddr = "111=111.111/111/24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), FAIL);
    ipAddr = "111.111.111.111=24";
    EXPECT_EQ(parser.isIpAddress(ipAddr), FAIL);
}

TEST(IpAddressParsing, BadSeparator)
{
    
}

// TEST(IpAddressParsing, BadSeparator)
// {
    
// }

// TEST(IpAddressParsing, BadSeparator)
// {
    
// }


// TEST(IpAddressParsing, BadSeparator)
// {
    
// }

// TEST(IpAddressParsing, BadSeparator)
// {
    
// }