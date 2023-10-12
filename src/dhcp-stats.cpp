#include <iostream>

#include "headers/argument_processor.hpp"
#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"

int main(int argc, char** argv)
{
    ArgumentProcessor* ap = new ArgumentProcessor();
    int32_t retCode = ap->processArguments(argc, argv);

    if (retCode == SYSTEM_ERR)
    {
        ap->closeFiles();
        delete ap;
        return EXIT_FAILURE;
    }
    else if (retCode == INVALID_CMDL_OPTIONS)
    {
        std::cerr << "Wrong command-line options." << std::endl;
        ap->printHelp();
        ap->closeFiles();
        delete ap;
        return EXIT_FAILURE;
    }
    
    PacketSniffer* ps = new PacketSniffer(ap->getInterface().data());
    retCode = ps->sniffPackets();

    if (retCode == FAIL) 
    {
        ap->closeFiles();
        delete ap;
        delete ps;
        return EXIT_FAILURE;
    }
  
    ap->closeFiles();
    delete ap;
    delete ps;
    return EXIT_SUCCESS;
}