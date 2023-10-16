#include <iostream>
#include <memory>

#include "headers/argument_processor.hpp"
#include "headers/packet_sniffer.hpp"
#include "headers/errors.h"

int main(int argc, char** argv)
{
    std::unique_ptr<ArgumentProcessor> ap = std::make_unique<ArgumentProcessor>();
    int32_t retCode = ap->processArguments(argc, argv);

    if (retCode == SYSTEM_ERR)
    {
        return EXIT_FAILURE;
    }
    else if (retCode == INVALID_CMDL_OPTIONS)
    {
        ap->printHelp();
        return EXIT_FAILURE;
    }
    
    std::unique_ptr<PacketSniffer> ps = std::make_unique<PacketSniffer>();
    ps->setInterface(ap->getInterface());
    ps->setInputFile(ap->getFileName());

    retCode = ps->setUpSniffing();
    if (retCode == FAIL) 
    {
        return EXIT_FAILURE;
    }

    ps->sniffPackets();
    
    return EXIT_SUCCESS;
}