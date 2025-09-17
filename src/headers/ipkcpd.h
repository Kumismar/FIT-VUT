#ifndef ipkcpd_h
#define ipkcpd_h

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
#include "message_parsing.h"
#include "constants.h"

/**
 * @brief Changes global variable `running` to false when program is interrupted, which makes the main loop of program quit.
 * It's purpose is to exit the program and properly close all connections, file descriptors and free memory.
 */
static void interruptHandler();

/**
 * @brief Prepares a socket file descriptor for communication with client.
 * 
 * The file descriptor is opened for either TCP or UDP communication, based on provided `mode`.
 * 
 * @param mode Mode in which the file descriptor is prepared.
 * @return File descriptor number.
 */
int openSocket(Modes mode);

/**
 * @brief Binds `address` to `socketFd` and checks if the operation was successful.
 * 
 * @param socketFd 
 * @param addrSize 
 * @param address 
 */
void bindAddressToSocket(int socketFd, socklen_t addrSize, struct sockaddr *address);

/**
 * @brief Converts `address` from string to in_addr_t (unsigned 32bit int).
 * 
 * The validity of `address` is checked in builtin `inet_pton()` function; 
 * see [man 3 inet_pton()](https://man7.org/linux/man-pages/man3/inet_pton.3.html).
 * 
 * 
 * @param address Address in string form.
 * @return Address in in_addr_t (e.g. unsigned 32bit int)
 */
in_addr_t getAddress(char *address);

/**
 * @brief Takes care of UDP communication with clients.
 * Recieves message, calls parsing function and sends a response to the same client.
 * 
 * @param socketFd Socket on which the server communicates. 
 * @param buffer Universal buffer for any communication with any client.
 * @param address Address on which the server communicates.
 * @param addressSize Length of address in bytes.
 */
void communicateWithUDP(int socketFd, char *buffer, struct sockaddr *address, socklen_t addressSize);

/**
 * @brief The main function for TCP communication. Every child process called from
 * `communicateWithTCP()` takes care of it's own client. Recieves messages, calls
 * parsing function and sends responses.
 * 
 * @param communicationSocket Socket on which the server communicates. 
 * @param buffer Universal buffer for any communication with any client.
 */
void childCommunicationTCP(int communicationSocket, char *buffer);

/**
 * @brief Takes care of TCP communication with clients.
 * Forks for every client connected to the server (up to 16 clients).
 * 
 * @param socketFd Socket on which the server communicates. 
 * @param buffer Universal buffer for any communication with any client.
 * @param address Address on which the server communicates.
 * @param addressSize Length of address in bytes.
 */
void communicateWithTCP(int socketFd, char *buffer, struct sockaddr *address, socklen_t addressSize);

#endif /* ipkcpd_h */