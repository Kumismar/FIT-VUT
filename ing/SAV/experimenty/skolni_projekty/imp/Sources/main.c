#include "MK60D10.h"
#include "headers/fsm.h"
#include "headers/calculator.h"
#include "headers/common.h"
#include "headers/uart.h"
#include "headers/result_array.h"
#include <stdio.h>
#include <string.h>

#define FIRST_OPERAND 1
#define SECOND_OPERAND 2

void InitPorts(void)
{
	// Select pins to GPIO, enable pull resistors, select pull up
	PORTA->PCR[8] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));
	PORTA->PCR[10] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));
	PORTA->PCR[11] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));

    PORTA->PCR[27] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));
	PORTA->PCR[29] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));
	PORTA->PCR[28] = (PORT_PCR_MUX(0x01) | PORT_PCR_PE(0x01) | PORT_PCR_PS(0x01));

    // Set pins as input
	PTA->PDDR &= ~((1 << 8) | (1 << 10) | (1 << 11) | (1 << 27) | (1 << 28) | (1 << 29));

	// Set beeper to GPIO output
	PORTA->PCR[4] = PORT_PCR_MUX(0x1);
	PTA->PDDR |= (1 << 4);

    // Set UART pins
    PORTE->PCR[8]  = PORT_PCR_MUX(0x03); // UART5_TX

    // Button set for input
    PORTE->PCR[11] = (PORT_PCR_MUX(0x01)); // SW6
    PTE->PDDR &= ~(1 << 11);
}

void MCUInit()
{
    // Select frequency and range for clock
    MCG_C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01));

    // Enable clock for ports and uart
    SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK ;
}

int main(void)
{
    MCUInit();
    InitPorts();
    UART5Init();
    InitResultArray();

    CalcData operand1 = {
        .faultType = NONE,
        .type = INT,
        .value.intResult = 0
    };
    CalcData operand2 = {
        .faultType = NONE,
        .type = INT,
        .value.intResult = 0
    };
    CalcData result = {
        .faultType = NONE,
        .type = INT,
        .value.intResult = 0
    };
    Operator operator = ADD;
    CalcState presentState = OPERAND1;
    CalcState nextState = OPERAND1;

    // Place cursor to the beggining, clear everything and place it to the beggining again
    SendStr("\e[H\e[2J\e[H");
    SendStr("\e[7m*******Gigachad Calculator*******\e[0m");

    while (1) {
        presentState = nextState;
        switch (presentState) {
            case OPERAND1: {
                SendStr("\r\nVyberte prvni operand: ");
                operand1 = PickNumber(FIRST_OPERAND);
                nextState = OPERATOR;
                delay(1000000); // For unwanted interaction with the same button twice
                break;
            }
            case OPERATOR: {
                SendStr("\r\nVyberte operator: ");
                operator = PickOperator();
                if (operator == SQRT) {
                	nextState = CALCULATE;
                	break;
                }
                nextState = OPERAND2;
                delay(1000000);
                break;
            }
            case OPERAND2: {
                SendStr("\r\nVyberte druhy operand: ");
                operand2 = PickNumber(SECOND_OPERAND);
                nextState = CALCULATE;
                break;
            }
            case CALCULATE: {
                result = CalculateResult(operand1, operator, operand2);
                DisplayResult(&result);
                nextState = OPERAND1;
                delay(1000000);
                break;
            }
        }
    }

    /* Never leave main */
    return 0;
}
