/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:29
 */

#pragma once

#include "grammar.hpp"

/**
 * @brief Factory class for creating grammars.
 */
class GrammarFactory
{
public:
    /**
     * @brief Creates a grammar based on index given.
     *
     * @param grammarNumber Index of grammar that will be created.
     * @return Base class pointer to the created grammar.
     */
    static Grammar* CreateGrammar(unsigned grammarNumber);
};