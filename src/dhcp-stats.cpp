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
        delete ap;
        return EXIT_FAILURE;
    }
    else if (retCode == INVALID_CMDL_OPTIONS)
    {
        ap->printHelp();
        delete ap;
        return EXIT_FAILURE;
    }
    
    PacketSniffer* ps = new PacketSniffer();
    ps->setInterface(ap->getInterface());
    ps->setInputFile(ap->getFileName());

    retCode = ps->setUpSniffing();
    if (retCode == FAIL) 
    {
        delete ap;
        delete ps;
        return EXIT_FAILURE;
    }

    ps->sniffPackets();
    
    delete ap;
    delete ps;
    return EXIT_SUCCESS;
}