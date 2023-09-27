#include <iostream>

#include "headers/argument_processor.hpp"

int main(int argc, char** argv)
{
    ArgumentProcessor* ap = new ArgumentProcessor();
    if (ap->processArguments(argc, argv) == -1)
    {
        std::cerr << "neco je spatne sefe" << std::endl;
        delete ap;
        return 1;
    }
    ap->printMembers();
    return 0;
}