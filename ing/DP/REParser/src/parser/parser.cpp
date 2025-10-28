/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "parser.hpp"

#include "lexer.hpp"

#include <memory>

REParser::REParser(std::string& inputRegex, std::ofstream& outputFile)
{
    m_regex = std::move(inputRegex);
    if (outputFile) {
        m_printAST = true;
    }
    m_lexer.Initialize(m_regex);
}

ASTNodePtr REParser::Parse()
{
    Token token = m_lexer.GetNextToken();
    while (token.type != TokenType::END_OF_INPUT) {
        token = m_lexer.GetNextToken();
    }
    return std::make_shared<ASTNode>();
}