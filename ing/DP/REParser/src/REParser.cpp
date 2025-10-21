#include "include/REParser.hpp"

REParser::REParser(std::string& regex, std::ofstream& outputFile)
{
    this->regex = std::move(regex);
    if (outputFile) {
        this->printAST = true;
    }
}

void REParser::Parse() { }