#ifndef errors_h
#define errors_h

/* Argument processing errors */

#define INVALID_ARG_NUM 10
#define INVALID_CMD_ARGS 11
#define INVALID_MODE 12
#define INVALID_IPV4 13
#define INVALID_PORT 14

/* Communication with server errors */

#define SOCKET_FD_ERROR 20
#define SENDTO_FAILED 21
#define RECVFROM_FAILED 22
#define CONNECT_FAILED 23

/* Internal program error */

#define PROGRAM_FAILURE 99


#endif /* errors_h */