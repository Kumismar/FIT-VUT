/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "REParser.hpp"

#include "Lexer.hpp"

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
    return std::make_shared<ASTNode>();
}