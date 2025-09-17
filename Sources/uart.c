#include "headers/uart.h"
#include "MK60D10.h"

void UART5Init() 
{
    UART5->C2  &= ~(UART_C2_TE_MASK);
    // Baud rate 115 200 Bd, 1 stop bit
    UART5->BDL =  0x1A; 
    // Oversampling ratio 16, match address mode disabled
    UART5->C4  =  0x0F; 
    // Transmitter enable and set interrupt
    UART5->C2  |= UART_C2_TE_MASK; 
}

void SendCh(char c)
{
    while( !(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK) );
    UART5->D = c;
}

void SendStr(char* s)
{
    unsigned int i = 0;
    while (s[i] != '\0') {
        SendCh(s[i++]);
    }
}