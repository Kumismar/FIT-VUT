/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-03-18 19:12
 * @ Modified time: 2024-05-02 10:29
 */

#include "grammar_factory.hpp"
#include "grammar_1.hpp"
#include "grammar_2.hpp"
#include "grammar_3.hpp"
#include "grammar_4.hpp"
#include "grammar_5.hpp"
#include "grammar_6.hpp"
#include "internal_error.hpp"


Grammar* GrammarFactory::CreateGrammar(unsigned grammarNumber)
{
    switch (grammarNumber) {
        case 1: {
            return new Grammar1();
        }
        case 2: {
            return new Grammar2();
        }
        case 3: {
            return new Grammar3();
        }
        case 4: {
            return new Grammar4();
        }
        case 5: {
            return new Grammar5();
        }
        case 6: {
            return new Grammar6();
        }
        default: {
            throw InternalError("Invalid grammar number: " + std::to_string(grammarNumber) + "\n");
        }
    }
}