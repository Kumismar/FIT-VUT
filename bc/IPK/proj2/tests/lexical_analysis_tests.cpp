#include "gtest/gtest.h"

extern "C" {
    #include "../src/headers/message_parsing.h"
    #include "../src/headers/constants.h"
};

TEST(LexicalAnalysisValid, RandomInput)
{
    char buffer[] = "012341)(*+-\t89716)";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_SUCCESS);
}

TEST(LexicalAnalysisValid, SimpleQuery)
{
    char buffer[] = "(/ 8 9)";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_SUCCESS);
}

TEST(LexicalAnalysisValid, SimpleQueryWithExpression)
{
    char buffer[] = "(/ 8 (+ 1 3))";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_SUCCESS);
}

TEST(LexicalAnalysisValid, ComplicatedQuery)
{
    char buffer[] = "(/ 8 (+ (* 7 3) (/ 6 (- 1 5))))";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_SUCCESS);
}

TEST(LexicalAnalysisInvalid, RandomInput)
{
    char buffer[] = "9817491BC90287491C091247";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_FAILED);
}

TEST(LexicalAnalysisInvalid, QueryWithMistake)
{
    char buffer[] = "(+ 2 (* (/ (! 3) 6) 2) 3)";
    EXPECT_TRUE(lexicalAnalysis(buffer) == LEX_FAILED);
}
