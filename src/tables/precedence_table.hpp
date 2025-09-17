/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-01 18:39
 */

#pragma once

#include "token.hpp"
#include <vector>

/**
 * @brief Item selector in precedence table row.
 */
class PrecRowAccessor
{
private:
    /**
     * @brief Precedence table row.
     */
    const std::vector<char>& row;

public:
    /**
     * @brief Construct a new PrecRowAccessor object.
     * @param r Precedence table row.
     */
    PrecRowAccessor(const std::vector<char>& r);

    /**
     * @brief Select item in precedence table row.
     * @param t Indexing token.
     * @return Operator relation.
     */
    char operator[](const Token& t);
};

/**
 * @brief Data structure representing precedence table.
 */
class PrecedenceTable
{
private:
    /**
     * @brief Precedence table.
     */
    static const std::vector<std::vector<char>> precTable;

public:
    /**
     * @brief Select row in precedence table.
     * @param t Indexing token.
     * @return Row accessor object which further selects item in the row.
     */
    PrecRowAccessor operator[](const Token& t) const;

    /**
     * @brief Map token type to index in precedence table.
     * @param t Indexing token from which type is extracted.
     * @return Index in precedence table.
     */
    static unsigned MapTokenToIndex(const Token& t);
};