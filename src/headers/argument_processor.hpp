#pragma once

#include <string>
#include <vector>

/**
 * @brief Class responsible for processing command-line arguments.
 */
class ArgumentProcessor
{
private:
    /** Network interface that program will listen on if `-i` option is present. */
    char* interface = nullptr;

    /** File name that program will try to read from if `-r` option is present. */
    char* inputFileName = nullptr;

    /**
     * Network IP addresses given to the program via command-line arguments.
     * Stored in format x.x.x.x/y, where 0 <= x <= 255 and 1 <= x <= 32.
     */
    std::vector<std::string> ipPrefixes;

    /**
     * @brief Reads file name from `optarg` variable and stores it in this->inputFileName.
     */
    void getFileNameFromArg();

    /**
     * @brief Reads interface name from `optarg` variable and stores it in this->interface.
     */
    void getInterfaceFromArg();

    int32_t processOptions(int32_t argc, char** argv);

    int32_t processIpPrefixes(int32_t argc, char **argv);
public:

    /**
     * @brief Destructor responsible for deallocating memory of fields that are c-style strings.
     */
    ~ArgumentProcessor();

    /**
     * @brief Getter for interface name.
     * @return Pointer to interface (c-style string).
     */
    char* getInterface();

    /**
     * @brief Getter for input file name.
     * @return Pointer to file name (c-style string).
     */
    char* getFileName();

    /**
     * @brief Processes program command-line arguments.
     *
     * Implemented with (`getopt()`)[https://www.man7.org/linux/man-pages/man3/getopt.3.html],
     * fills in interface name or file name and checks if given IP Prefixes are valid
     * (uses instance of IP Address Parser for it).
     *
     * @param argc Number of arguments given to the program.
     * @param argv Argument vector given to the program.
     * @return SUCCESS (0) when all the arguments were valid.
     * @return SYSTEM_ERR (10) for extreme situations (std::stringstream error, operator>> error, etc.),
     * where neither user nor author are responsible for what happened.
     * @return INVALID_CMDL_ARGS (11) when any of the arguments was invalid.
     */
    int32_t processArguments(int argc, char** argv);

    /**
     * @brief Prints usage of program to stdout.
     */
    void printHelp();

    /**
     * @brief Getter for IP prefixes given to the program.
     *
     * Return pointer to the vector because vector of strings can get quite big in size.
     *
     * @return Pointer to vector of IP prefixes.
     */
    std::vector<std::string>* getIpPrefixes();
};