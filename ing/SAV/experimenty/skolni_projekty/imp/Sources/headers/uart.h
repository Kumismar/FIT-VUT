#ifndef uart_h
#define uart_h

/**
 * @brief Initialize UART5 transmitter.
 */
void UART5Init();

/**
 * @brief Send character via UART5 transmitter.
 * 
 * @param c Character to be sent.
 */
void SendCh(char c);

/**
 * @brief Send string of characters via UART5 transmitter.
 * Calls SendCh() for every character.
 * 
 * @param s String to be sent.
 */
void SendStr(char* str);

#endif