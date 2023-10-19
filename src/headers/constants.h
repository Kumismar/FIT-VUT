#pragma once

/* argument_processor constants */
#define SYSTEM_ERR 10
#define INVALID_CMDL_OPTIONS 11
#define MIN_ARGUMENTS 4

/* ip_address_manager constants */
#define MAX_MASK_NUMBER 32
#define NO_ADDRESSES_TAKEN 0
#define NO_UTILIZATION 0.0
#define NETWORK_AND_BROADCAST 2
#define CONVERT_TO_PERCENT 100
#define MASK_LENGTH 3
#define HALF_NETWORK_FULL 50

/* ip_address_parser constants */
#define STRING_START 0
#define CORRECT_IP_ADDR_BYTE_COUNT 4
#define MAX_MASK_NUMBER 32
#define MIN_MASK_NUMBER 1
#define MIN_BYTE_VALUE 0
#define MAX_BYTE_VALUE 255
#define DELIMITER 1

/* packet_sniffer constants */
#define NO_OPTIMIZATION 0
#define PROMISC 1
#define CLIENT_IPADDR_POSITION 16
#define MESSAGE_TYPE_LOCATION 240
#define DHCP 53
#define DHCP_TYPE_LOCATION 242
#define ACK 5
#define TIMEOUT_MS 10000
#define BYTES_PER_WORD 4
#define PACKET_SUCCESSFULLY_READ 1
#define CAPTURE_TIMEOUT 0

/* General constants needed for program functionality */
#define SUCCESS 0
#define FAIL -1