#ifndef ipkcpc_h
#define ipkcpc_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "argument_processing.h"
#include "errors.h"
#include "list.h"

#define OPCODE_AND_LENGTH 2
#define SKIP_OPCODE_AND_LENGTH 2
#define SKIP_OPCODE_STATUS_AND_LENGTH 3

/**
 * @brief This function handles the SIGINT signal so the program closes appropriately.
 * The mechanism is in global volatile variable 'running' that changes to false.
 * The main program functionality runs in a while loop and the entry condition
 * (among others) is that 'running' variable is set to true.
 */
static void interruptHandler();

/**
 * @brief Opens a socket depending on the mode given by user and checks 
 * the validity of returned value. Exits the application on failure.
 * The type of address used is IPv4.
 * 
 * @param mode Specifies if the socket will be opened in tcp or udp mode. 
 * 
 * @return File descriptor for the new created socket or -1 in case of failure.
 */
int openSocket(Modes mode);

/**
 * @brief Gets IPv4 address from given string @param address.
 * 
 * @return Address as in_addr_t (which is unsigned int). 
 */
in_addr_t getAddress(char *address);

/**
 * @brief Communicates with remote server in UDP mode.
 * 
 * Reads input from stdin, stores one line to @param buffer. There's also pointer called payloadData, 
 * which points to the third byte of the buffer. The first and second byte are reserved for opcode
 * and payload data length. 
 * 
 * Buffer is then sent to server with @param address.
 * 
 * The function waits for a response from the server, clearing the buffer before waiting.
 * After the data have arrived, they are printed to stdout, with prefix depending on
 * type of the response. 
 * 
 * @param addressSize Size of the structure where the address is loaded, some syscalls require it.
 * @param maxBufferLength 
 */
void communicateWithUDP(struct sockaddr* address, socklen_t addressSize, char* buffer, size_t maxBufferLength);

/**
 * @brief Communicates with remote server in TCP mode.
 * 
 * Opens a socket in TCP mode and connects to the server.
 * 
 * Reads input from stdin and stores one line to @param buffer. Unlike on UDP protocol, there are no
 * auxiliary pointers to the buffer because the protocol works with textual form of data. 
 * 
 * The client sends data read from stdin, waits for response from server and prints the data to stdout.
 * 
 * If the data recieved is string "BYE", then the connection is shut down and socket is closed. 
 * 
 * @param address Address of remote server.
 * @param addressSize Size of structure containing address, requied by some syscalls.
 * @param maxBufferLength 
 */
void communicateWithTCP(struct sockaddr* address, socklen_t addressSize, char* buffer, size_t maxBufferLength);


#endif /* ipkcpc.h */