#include "headers/common.h"
#include "MK60D10.h"

void delay(int t1)
{
	for(int i=0; i<t1; i++);
}

void beep(int length, int depth)
{
	// same length for encoder
	if (depth == CW) {
		length /= 3;
	}
	else if (depth == BTN) {
		length = length * 2 / 3;
	}

	for (int i=0; i<length; i++) {
		PTA->PDOR |=  BEEP_MASK; delay(depth);
		PTA->PDOR &= ~BEEP_MASK; delay(depth);
	}
}
