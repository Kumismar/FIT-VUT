#include <iostream>
#include <memory>
#include <ncurses.h>
#include <sys/syslog.h>

#include "headers/argument_processor.hpp"
#include "headers/packet_sniffer.hpp"
#include "headers/constants.h"

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
    else if (retCode == WANTS_HELP)
    {
        ap->printHelp();
        return EXIT_SUCCESS;
    }

    std::unique_ptr<PacketSniffer> ps = std::make_unique<PacketSniffer>();
    ps->setInterface(ap->getInterface());
    ps->setInputFile(ap->getFileName());

    retCode = ps->setUpSniffing();
    if (retCode == FAIL) 
    {
        return EXIT_FAILURE;
    }

    initscr();
    std::vector<std::string>* ipAddresses = ap->getIpPrefixes();
    retCode = ps->sniffPackets(*ipAddresses);
    ps->cleanUp();
    if (retCode == FAIL)
    {
        endwin();
        return EXIT_FAILURE;
    }
    printw("Press any key to exit");
    refresh();
    getch();
    endwin();
    return EXIT_SUCCESS;
}