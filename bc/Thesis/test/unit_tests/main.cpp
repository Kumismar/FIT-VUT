/**
 * @ Author: Ondřej Koumar
 * @ Email: xkouma02@stud.fit.vutbr.cz
 * @ Create Time: 2024-04-29 11:26
 * @ Modified time: 2024-04-29 11:39
 */

#include "ast.hpp"
#include "grammar_1.hpp"
#include "grammar_2.hpp"
#include "grammar_3.hpp"
#include "grammar_4.hpp"
#include "grammar_5.hpp"
#include "grammar_6.hpp"
#include "logger.hpp"

#include <gtest/gtest.h>


void Cleanup()
{
    Grammar1::Cleanup();
    Grammar2::Cleanup();
    Grammar3::Cleanup();
    Grammar4::Cleanup();
    Grammar5::Cleanup();
    Grammar6::Cleanup();
    Logger::Cleanup();
    AST::Cleanup();

    if (!inputTape.empty()) {
        for (const auto& token: inputTape) {
            delete token;
        }
        inputTape.clear();
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int retCode = RUN_ALL_TESTS();
    Cleanup();
    return retCode;
}