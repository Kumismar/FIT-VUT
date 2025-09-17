#include "gtest/gtest.h"

extern "C" {
    #include "../headers/ipkcpc.h"
    #include "../headers/errors.h"
    #include "../headers/argument_processing.h"
    #include "../headers/list.h"
    #include "../headers/error_functions.h"
};



/*   IP ADDRESS TESTS   */


TEST(IPAddrTest, correctIPAddresses)
{
    initList();
    char correctIPAddr[16] = "123.123.123.123";
    EXPECT_STREQ(parseIPAddress(correctIPAddr), correctIPAddr);

    strcpy(correctIPAddr, "255.255.255.255");
    EXPECT_STREQ(parseIPAddress(correctIPAddr), correctIPAddr);

    strcpy(correctIPAddr, "10.20.30.40");
    EXPECT_STREQ(parseIPAddress(correctIPAddr), correctIPAddr);

    strcpy(correctIPAddr, "0.255.127.0");
    EXPECT_STREQ(parseIPAddress(correctIPAddr), correctIPAddr);

    strcpy(correctIPAddr, "0.0.0.0");
    EXPECT_STREQ(parseIPAddress(correctIPAddr), correctIPAddr);
    disposeList();
}

TEST(IPAddrTest, threePartsOnly)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"122.233.432"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, fiveParts)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"123.123.123.123.123"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, randomStrings)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"als23ij4h 2"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    EXPECT_EXIT(parseIPAddress((char*)"1o382cb7tro8 b1b2"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, stringsSeparatedWithDots)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"asf.hrj.rfs.asd"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, addressWithoutDots)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"122233432123"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, specialCharacters)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"*.9@.#@.(*&)"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    EXPECT_EXIT(parseIPAddress((char*)"!(*@&#9123)"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, dotInTheEnd)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"123.123.123.123."), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, emptyString)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)""), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, valueOutOfRange)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"1.2.3.256"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, negativeNumber)
{  
    initList(); 
    EXPECT_EXIT(parseIPAddress((char*)"1.2.-3.4"), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}

TEST(IPAddrTest, threeDotsWithoutNumbers)
{
    initList();
    EXPECT_EXIT(parseIPAddress((char*)"..."), testing::ExitedWithCode(INVALID_IPV4), ".*");
    disposeList();
}



/*    MODE TESTS    */



TEST(modeTest, validCases)
{
    EXPECT_EQ(getMode((char*)"tcp"), TCP);
    EXPECT_EQ(getMode((char*)"TCP"), TCP);
    EXPECT_EQ(getMode((char*)"udp"), UDP);
    EXPECT_EQ(getMode((char*)"UDP"), UDP);
}

TEST(modeTest, lowerAndUpperCase)
{
    EXPECT_EXIT(getMode((char*)"tCp"), testing::ExitedWithCode(INVALID_MODE), ".*");
    EXPECT_EXIT(getMode((char*)"Udp"), testing::ExitedWithCode(INVALID_MODE), ".*");
}

TEST(modeTest, randomStrings)
{
    EXPECT_EXIT(getMode((char*)"23lk4j5h askjldfh"), testing::ExitedWithCode(INVALID_MODE), ".*");
    EXPECT_EXIT(getMode((char*)"lkj"), testing::ExitedWithCode(INVALID_MODE), ".*");
}

TEST(modeTest, emptyString)
{
    EXPECT_EXIT(getMode((char*)""), testing::ExitedWithCode(INVALID_MODE), ".*");
}

TEST(modeTest, asciiValuesOfLetters)
{
    EXPECT_EXIT(getMode((char*)"856880"), testing::ExitedWithCode(INVALID_MODE), ".*");
}

TEST(modeTest, specialCharacters)
{
    EXPECT_EXIT(getMode((char*)"*)(*)!@&#"), testing::ExitedWithCode(INVALID_MODE), ".*");
}



/*   PORT TESTS   */



TEST(portTest, correctPorts)
{
    EXPECT_EQ(getPort((char*)"8000"), 8000);
    EXPECT_EQ(getPort((char*)"9083"), 9083);
    EXPECT_EQ(getPort((char*)"0"), 0);  
    EXPECT_EQ(getPort((char*)"65535"), 65535);
}

TEST(portTest, randomStrings)
{
    EXPECT_EXIT(getPort((char*)" jh42k12"), testing::ExitedWithCode(INVALID_PORT), ".*");
    EXPECT_EXIT(getPort((char*)"iul 4 kl14"), testing::ExitedWithCode(INVALID_PORT), ".*");
}

TEST(portTest, emptyString)
{
    EXPECT_EXIT(getPort((char*)""), testing::ExitedWithCode(INVALID_PORT), ".*");
}

TEST(portTest, tooBigNumbers)
{
    EXPECT_EXIT(getPort((char*)"17823987"), testing::ExitedWithCode(INVALID_PORT), ".*");
    EXPECT_EXIT(getPort((char*)"65536"), testing::ExitedWithCode(INVALID_PORT), ".*");
}

TEST(portTest, negativeNumbers)
{
    EXPECT_EXIT(getPort((char*)"-3"), testing::ExitedWithCode(INVALID_PORT), ".*");
    EXPECT_EXIT(getPort((char*)"-987"), testing::ExitedWithCode(INVALID_PORT), ".*");
}

TEST(portTest, specialCharacters)
{
    EXPECT_EXIT(getPort((char*)"123*(&)"), testing::ExitedWithCode(INVALID_PORT), ".*");
    EXPECT_EXIT(getPort((char*)"&(2289#*&)"), testing::ExitedWithCode(INVALID_PORT), ".*");
}


/* WRONG NUMBER OF CMD ARGS TESTS */

TEST(oneArgMoreTimes, twoModesOnly)
{
    isUsedArg((char*)"-m");
    EXPECT_EQ(isUsedArg((char*)"-m"), true);
}

TEST(oneArgMoreTimes, allArgsOneTwice)
{
    isUsedArg((char*)"-m");
    isUsedArg((char*)"-h");
    isUsedArg((char*)"-p");
    EXPECT_EQ(isUsedArg((char*)"-m"), true);
    EXPECT_EQ(isUsedArg((char*)"-h"), true);
    EXPECT_EQ(isUsedArg((char*)"-p"), true);
}

TEST(oneArgMoreTimes, twoArgsOneTwice)
{
    isUsedArg((char*)"-m");
    isUsedArg((char*)"-h");
    EXPECT_EQ(isUsedArg((char*)"-h"), true);
}

TEST(correctArgs, oneByOne)
{
    EXPECT_EQ(isUsedArg((char*)"-m"), true);
    EXPECT_EQ(isUsedArg((char*)"-p"), true);
    EXPECT_EQ(isUsedArg((char*)"-h"), true);
}

TEST(correctArgs, twoRemaining)
{
    isUsedArg((char*)"-p");
    EXPECT_EQ(isUsedArg((char*)"-m"), true);
    EXPECT_EQ(isUsedArg((char*)"-h"), true);
}

TEST(correctArgs, oneRemaining)
{
    isUsedArg((char*)"-h");
    isUsedArg((char*)"-m");
    EXPECT_EQ(isUsedArg((char*)"-p"), true);
}

int main(int argc, char **argv)
{
    initList();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}