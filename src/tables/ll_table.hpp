/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-01 18:42
 */

#pragma once

#include "nonterminal.hpp"
#include "table_index.hpp"
#include "token.hpp"
#include <vector>

/**
 * @brief Item selector in LL table row.
 */
class LLRowAccessor
{
private:
    /**
     * @brief LL table row.
     */
    const std::vector<LLTableIndex>& row;

public:
    /**
     * @brief Construct a new LLRowAccessor object.
     * @param r LL table row.
     */
    LLRowAccessor(const std::vector<LLTableIndex>& r);

    /**
     * @brief Select item in LL table row.
     * @param t Indexing token.
     * @return An ordered pair of component and rule index.
     */
    LLTableIndex operator[](const Token& t) const;
};

/**
 * @brief Data structure representing LL table.
 */
class LLTable
{
private:
    /**
     * @brief LL table.
     */
    static const std::vector<std::vector<LLTableIndex>> table;

public:
    /**
     * @brief Select row in LL table.
     * @param nt Indexing nonterminal.
     * @return Row accessor object which further selects item in the row.
     */
    LLRowAccessor operator[](const Nonterminal& nt) const;
};