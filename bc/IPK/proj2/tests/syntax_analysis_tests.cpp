#include "gtest/gtest.h"

extern "C" {
    #include "../src/headers/message_parsing.h"
    #include "../src/headers/constants.h"
    #include "../src/headers/stack.h"
    #include "../src/headers/pointerList.h"
}

TEST(ValidSyntax, SimpleQuery)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 2)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(ValidSyntax, SimpleQueryWithExpression)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 (- 5 7))";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(ValidSyntax, ComplexQuery)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ (/ 4 (- 8 3) ) (/ (- (/ 2 3) (* 1 3)) (* 3 4) ) )";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(ValidSyntax, NumbersWithMoreDigitsSimpleQuery)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 134 290)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(ValidSyntax, NumbersWithMoreDigitsComplexQuery)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ (* (/ 7 (* 69 420) ) 3) (- 9 (- 12 193) ) )";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(ValidSyntax, QueryWithTabAndTwoSpaces)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 \t 2)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_SUCCESS);

    disposeList();
}

TEST(InvalidSyntax, NegativeNumber)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 -2)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_FAILED);

    disposeList();
}

TEST(InvalidSyntax, TooMuchParentheses)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 (- 2 (2)))";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_FAILED);

    disposeList();
}

TEST(InvalidSyntax, ThreeOperands)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1 2 3)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_FAILED);

    disposeList();
}

TEST(InvalidSyntax, UnexpectedNewline)
{
    initList();
    Token token;
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);

    Stack stack;
    initStack(&stack);

    char query[] = "(+ 1\n -2)";
    getNextToken(query, &token, true);
    EXPECT_EQ(parseQuery(query, &token, &stack), PARSE_FAILED);

    disposeList();
}