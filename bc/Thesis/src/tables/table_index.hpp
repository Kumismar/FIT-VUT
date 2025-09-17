/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-01 18:55
 */

#pragma once

/**
 * @brief Data structure representing LL table entry.
 * It's an ordered pair of component and rule number that specifies rule that will be expanded onto pushdown.
 */
typedef struct llTableIndex {
    /**
     * @brief Grammar number.
     */
    unsigned grammarNumber;

    /**
     * @brief Rule number.
     */
    unsigned ruleNumber;

    /**
     * @brief Compare two LL table indices.
     * @param other Other LL table index.
     * @return True if both numbers in the pair are equal, false otherwise.
     */
    bool operator!=(const struct llTableIndex& other) const
    {
        return ((this->grammarNumber != other.grammarNumber) || (this->ruleNumber != other.ruleNumber));
    }
} LLTableIndex;