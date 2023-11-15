#include <ncurses.h>
#include <csignal>
#include <iostream>

#include "headers/argument_processor.hpp"
#include "headers/packet_sniffer.hpp"
#include "headers/constants.h"
#include "headers/AllocList.hpp"

std::list<ListInsertable*> AllocList;

void deleteAllAndExit(int32_t exitCode)
{
    deleteAll();
    exit(exitCode);
}

void signalHandler(int signum)
{
    std::cout << "Recieved signal: " << signum << ". Gracefully exiting." << std::endl;
    deleteAllAndExit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    ArgumentProcessor* ap = new ArgumentProcessor();
    int32_t retCode = ap->processArguments(argc, argv);

    if (retCode == SYSTEM_ERR)
    {
        deleteAllAndExit(EXIT_FAILURE);
    }
    else if (retCode == INVALID_CMDL_OPTIONS)
    {
        ap->printHelp();
        deleteAllAndExit(EXIT_FAILURE);
    }
    else if (retCode == WANTS_HELP)
    {
        ap->printHelp();
        deleteAllAndExit(EXIT_SUCCESS);
    }

    PacketSniffer* ps = new PacketSniffer();
    ps->setInterface(ap->getInterface());
    ps->setInputFile(ap->getFileName());
    retCode = ps->setUpSniffing();
    if (retCode == FAIL) 
    {
        deleteAllAndExit(EXIT_FAILURE);
    }

    initscr();
    std::vector<std::string> ipAddresses = ap->getIpPrefixes(); 
    retCode = ps->sniffPackets(ipAddresses);
    if (retCode == FAIL)
    {
        endwin();
        deleteAllAndExit(EXIT_FAILURE);
    }
    printw("Press any key to exit");
    refresh();
    getch();
    endwin();
    deleteAllAndExit(EXIT_SUCCESS);
}