/**
 * @file ip_address_parser.hpp
 * @author Ondřej Koumar (xkouma02@stud.fit.vutbr.cz)
 */

#pragma once

#include <string>

#include "list_insertable.hpp"

/**
 * @brief Is responsible for parsing IP addresses given as command-line arguments.
 */
class IpAddressParser : public ListInsertable
{
private:
    /** Auxiliary field for saving IP address tokens which are used in multiple methods. */
    std::string token;

    /**
     * @brief Checks if mask number is within range.
     *
     * Reads mask from this->token.
     *
     * @return SUCCESS (0) when mask is within range.
     * @return FAILURE (-1) when mask is not within range.
     */
    int32_t parseMask();

    /**
     * @brief Checks if a byte from IP Address is within range.
     *
     * Reads the byte from this->token.
     *
     * @return SUCCESS (0) when byte is within range.
     * @return FAILURE (-1) when byte is not within range.
     */
    int32_t parseByte();

public:

    /**
     * Inserts new object to AllocList.
     */
    IpAddressParser();

    /**
     * @brief Parses given ipAddr in x.x.x.x/y format (with mask).
     * @param ipAddr IP Address with mask.
     * @return SUCCESS (0) on successful parsing
     * @return FAILURE (-1) when ipAddr is in incorrect format.
     */
    int32_t parseIPAddress(std::string ipAddr);
};