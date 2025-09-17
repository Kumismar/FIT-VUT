#ifndef constants_h
#define constants_h

#define OPCODE 0
#define STATUS_CODE 1
#define LENGTH 1
#define MSGLENGTH 2
#define OPCODE_AND_MSGLENGTH 2
#define SKIP_OPCODE_MSGLENGTH 2
#define OPCODE_STATUS_LENGTH 3
#define SKIP_OPCODE_STATUS_LENGTH 3

#define LEX_SUCCESS 0
#define LEX_FAILED -1
#define PARSE_SUCCESS 0
#define PARSE_FAILED -1

#define MAX_CONNECTIONS 16

#define MAX_BUFFER_LENGTH 256

#endif /* constant_h */