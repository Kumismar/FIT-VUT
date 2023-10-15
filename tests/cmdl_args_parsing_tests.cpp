#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

#include "../src/headers/argument_processor.hpp"
#include "../src/headers/ip_address_parser.hpp"
#include "../src/headers/errors.h"
#include "aux/test_utils.hpp"

TEST(ArgumentProcessing, CorrectArguments)
{
    std::ofstream file("test.txt");
    ArgumentProcessor ap;
    std::vector<std::string> args;
    uint8_t argc;
    char** argv;

    // test1
    argc = 4;
    args = {
        "./dhcp-stats",
        "-r",
        "test.txt",
        "0.0.0.0/22"
    }; 
    argv = stringVecToCharArr(args);
    EXPECT_EQ(ap.processArguments(argc, argv), SUCCESS);

    // test2
    argc = 4;
    args = {
        "./dhcp-stats",
        "-i",
        "eth0",
        "1.1.1.1/1"
    };
    argv = stringVecToCharArr(args);
    EXPECT_EQ(ap.processArguments(argc, argv), SUCCESS);

    // test3
    argc = 6;
    args = {
        "./dhcp-stats",
        "-i",
        "eth0",
        "-r",
        "test.txt",
        "1.1.1.1/1"
    };
    argv = stringVecToCharArr(args);
    EXPECT_EQ(ap.processArguments(argc, argv), SUCCESS);

    deleteCharArr(argv, argc);
    std::remove("test.txt");
}

TEST(ArgumentProcessing, MissingIpPrefixes)
{
    std::ofstream file("test.txt");
    ArgumentProcessor ap;
    std::vector<std::string> args;
    uint8_t argc;
    char** argv;

    // test1
    argc = 3;
    args = {
        "./dhcp-stats",
        "-r",
        "test.txt",
    }; 
    argv = stringVecToCharArr(args);
    std::cout << ap.processArguments(argc, argv) << std::endl;
    EXPECT_EQ(ap.processArguments(argc, argv), INVALID_CMDL_OPTIONS);

    // test2
    argc = 5;
    args = {
        "./dhcp-stats",
        "-r",
        "test.txt",
        "-i",
        "eth0"
    }; 
    argv = stringVecToCharArr(args);
    EXPECT_EQ(ap.processArguments(argc, argv), INVALID_CMDL_OPTIONS);

    // test3
    argc = 3;
    args = {
        "./dhcp-stats",
        "-i",
        "eth0"
    }; 
    argv = stringVecToCharArr(args);
    EXPECT_EQ(ap.processArguments(argc, argv), INVALID_CMDL_OPTIONS);

    deleteCharArr(argv, argc);
    std::remove("test.txt");
}

TEST(ArgumentProcessing, NoInterfaceNoFile)
{
    std::ofstream file("test.txt");
    ArgumentProcessor ap;
    std::vector<std::string> args;
    uint8_t argc;
    char** argv;

    // test1
    argc = 6;
    args = {
        "./dhcp-stats",
        "0.0.0.0/21",
        "0.0.0.0/21",
        "0.0.0.0/21",
        "0.0.0.0/21",
        "0.0.0.0/21",
    }; 
    argv = stringVecToCharArr(args);
    int32_t retCode = ap.processArguments(argc, argv);
    EXPECT_EQ(retCode, INVALID_CMDL_OPTIONS);

    deleteCharArr(argv, argc);
    std::remove("test.txt");
}