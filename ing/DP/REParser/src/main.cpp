/**
 * @ Author: Ondřej Koumar
 * @ Project: REParser
 * @ Create Date: 2025-10-21
 * @ Description:
 */

#include "CLI11.hpp"
#include "REParser.hpp"
#include "REParserException.hpp"

#include <cstdlib>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
    CLI::App app { "Regular expression parser with AST" };

    std::string inputFileName;
    app.add_option<std::string>("-i", inputFileName, "Input file");
    std::string outputFileName;
    app.add_option<std::string>("-o", outputFileName, "AST output file");

    CLI11_PARSE(app, argc, argv);

    std::string stringRegex;
    if (inputFileName.empty()) {
        std::getline(std::cin, stringRegex);
    }
    else {
        std::ifstream inputFile(inputFileName, std::ios::in);
        if (!inputFile) {
            std::cerr << "Failed to open input file \"" << inputFileName << "\"" << std::endl;
            return EXIT_FAILURE;
        }

        std::getline(inputFile, stringRegex);
    }

    std::ofstream outputFile(outputFileName);

    try {
        REParser parser(stringRegex, outputFile);
        parser.Parse();
    }
    catch (const REParserException& e) {
        std::cerr << "A problem occured during parsing: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}