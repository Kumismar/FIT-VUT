/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#pragma once

#include "ASTNode.hpp"
#include "Lexer.hpp"

#include <fstream>
#include <memory>
#include <string>

using ASTNodePtr = std::shared_ptr<ASTNode>;

class REParser
{
private:
    std::string m_regex;
    bool m_printAST = false;
    Lexer m_lexer;

public:
    REParser(std::string& inputRegex, std::ofstream& outputFile);
    ASTNodePtr Parse();
};