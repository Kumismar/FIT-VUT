#include <fstream>
#include <string>

class REParser
{
private:
    std::string regex;
    bool printAST = false;

public:
    REParser(std::string& regex, std::ofstream& outputFile);
    void Parse();
};