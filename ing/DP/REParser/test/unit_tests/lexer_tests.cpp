//
// Created by Ondrej on 29/10/2025.
//

#include "LexicalError.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"
#include "lexer.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Empty and single symbol regexes", "[Lexer]")
{
    Lexer lexer;
    SECTION("Empty regex")
    {
        std::string regex = "";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::END_OF_INPUT);
        REQUIRE(token.text == "");
    }
    SECTION("One literal")
    {
        std::string regex = "a";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::LITERAL);
        REQUIRE(token.text == "a");
    }
    SECTION("Alternation")
    {
        std::string regex = "|";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::ALTERNATION);
        REQUIRE(token.text == "|");
    }
    SECTION("Anchor start")
    {
        std::string regex = "^";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::ANCHOR_START);
        REQUIRE(token.text == "^");
    }
    SECTION("Anchor end")
    {
        std::string regex = "$";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::ANCHOR_END);
        REQUIRE(token.text == "$");
    }
    SECTION("Capture group start")
    {
        std::string regex = "(";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::GROUP_START);
        REQUIRE(token.text == "(");
    }
    SECTION("Capture group end")
    {
        std::string regex = ")";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::GROUP_END);
        REQUIRE(token.text == ")");
    }
    SECTION("Non-capturing group start")
    {
        std::string regex = "(?:";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::GROUP_NONCAPTURE_START);
        REQUIRE(token.text == "(?:");
    }
    SECTION("Lookahead positive start")
    {
        std::string regex = "(?=";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::LOOKAHEAD_POSITIVE_START);
        REQUIRE(token.text == "(?=");
    }
    SECTION("Lookahead negative start")
    {
        std::string regex = "(?!";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::LOOKAHEAD_NEGATIVE_START);
        REQUIRE(token.text == "(?!");
    }
    SECTION("Lookbehind positive start")
    {
        std::string regex = "(?<=";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::LOOKBEHIND_POSITIVE_START);
        REQUIRE(token.text == "(?<=");
    }
    SECTION("Lookbehind negative start")
    {
        std::string regex = "(?<!";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::LOOKBEHIND_NEGATIVE_START);
        REQUIRE(token.text == "(?<!");
    }
    SECTION("Named group start")
    {
        std::string regex = "(?<name>";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::GROUP_NAMED_START);
        REQUIRE(token.text == "(?<name>");
    }
    SECTION("Character class start")
    {
        std::string regex = "[";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::CHAR_CLASS_START);
        REQUIRE(token.text == "[");
    }
    SECTION("Character class end")
    {
        std::string regex = "[]";
        lexer.Initialize(regex);
        lexer.GetNextToken();
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::CHAR_CLASS_END);
        REQUIRE(token.text == "]");
    }
    SECTION("Character class hyphen")
    {
        std::string regex = "[A-";
        lexer.Initialize(regex);
        lexer.GetNextToken();
        lexer.GetNextToken();
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::CHAR_CLASS_RANGE);
        REQUIRE(token.text == "-");
    }
    SECTION("Character class negation")
    {
        std::string regex = "[^";
        lexer.Initialize(regex);
        lexer.GetNextToken();
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::CHAR_CLASS_NEGATION);
        REQUIRE(token.text == "^");
    }
    SECTION("Digit character class")
    {
        std::string regex = "\\d";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::DIGIT_CLASS);
        REQUIRE(token.text == "\\d");
    }
    SECTION("Non-digit character class")
    {
        std::string regex = "\\D";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::NON_DIGIT_CLASS);
        REQUIRE(token.text == "\\D");
    }
    SECTION("Word character class")
    {
        std::string regex = "\\w";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::WORD_CHAR_CLASS);
        REQUIRE(token.text == "\\w");
    }
    SECTION("Non-word character class")
    {
        std::string regex = "\\W";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::NON_WORD_CHAR_CLASS);
        REQUIRE(token.text == "\\W");
    }
    SECTION("Whitespace character class")
    {
        std::string regex = "\\s";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::WHITESPACE_CLASS);
        REQUIRE(token.text == "\\s");
    }
    SECTION("Non-whitespace character class")
    {
        std::string regex = "\\S";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::NON_WHITESPACE_CLASS);
        REQUIRE(token.text == "\\S");
    }
    SECTION("Back reference")
    {
        std::string regex = "\\7";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::BACKREFERENCE);
        REQUIRE(token.text == "\\7");
    }
    SECTION("Named back reference")
    {
        std::string regex = "\\k<name>";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::NAMED_BACKREFERENCE);
        REQUIRE(token.text == "\\k<name>");
    }
    SECTION("Word boundary")
    {
        std::string regex = "\\b";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::ANCHOR_WORD_BOUNDARY);
        REQUIRE(token.text == "\\b");
    }
    SECTION("Non-word boundary")
    {
        std::string regex = "\\B";
        lexer.Initialize(regex);
        Token token = lexer.GetNextToken();
        REQUIRE(token.type == TokenType::ANCHOR_NONWORD_BOUNDARY);
        REQUIRE(token.text == "\\B");
    }
}

TEST_CASE("Lexer error handling", "[Lexer]")
{
    using namespace Catch::Matchers;
    Lexer lexer;
    SECTION("Unclosed capture group name")
    {
        std::string regex = "(?<name";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Unclosed group capture name"));
    }
    SECTION("Invalid characters in capture group name")
    {
        std::string regex = "(?<n\ame";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Invalid character in capture group name"));
    }
    SECTION("Unfinished '(?<' sequence")
    {
        std::string regex = "(?<";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Unfinished sequence '(?<'"));
    }
    SECTION("Invalid group identifier")
    {
        std::string regex = "(?1";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Invalid group indentifier"));
    }
    SECTION("Unfinished escape sequence")
    {
        std::string regex = "\\";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Unfinished escape sequence"));
    }
    SECTION("Unfinished '(?' sequence")
    {
        std::string regex = "(?";
        lexer.Initialize(regex);
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Unfinished sequence '(?'"));
    }
    SECTION("Unfinished escape sequence in character class")
    {
        std::string regex = "[\\";
        lexer.Initialize(regex);
        lexer.GetNextToken();
        REQUIRE_THROWS_MATCHES(lexer.GetNextToken(),
                               LexicalError,
                               ContainsSubstring("Unfinished escape sequence"));
    }
}