#include "headers/fsm.h"
#include "MK60D10.h"

BoardInteraction ReadBoardInteraction(int encoderNum) 
{
    State nextState = IDLE;
    State presentState = IDLE;

    int a = 0, b = 0, btn = 0, btn2 = 0, clr_btn = 0;
    // default encoder 1
    int pin_a = 8, pin_b = 10, pin_btn1 = 11, pin_btn2 = 28, pin_clr_btn = 11;

    if (encoderNum == PICK_FROM_PREVIOUS) {
        pin_a = 27;
        pin_b = 29;
        pin_btn1 = 28;
    }

    while (1) {
        a = !(PTA->PDIR & (1 << pin_a));
        b = !(PTA->PDIR & (1 << pin_b));
        btn = !(PTA->PDIR & (1 << pin_btn1));
        clr_btn = !(PTE->PDIR & (1 << pin_clr_btn));

        if (encoderNum == CHOOSE_OPERAND_OR_OPERATOR) {
            btn2 = !(PTA->PDIR & (1 << pin_btn2));
        }
        presentState = nextState;
        switch (presentState) {
            // Nothing has moved yet
            case IDLE: {
                if (btn) {
                    nextState = EN_BTN_PRESSED;
                    break;
                }

                if (btn2) {
                    nextState = EN2_BTN_PRESSED;
                    break;
                }
                
                if (a) {
                    nextState = EN_CCW1;
                    break;
                }

                if (b) {
                    nextState = EN_CW1;
                    break;
                }

                if (clr_btn) {
                    nextState = CLR_BTN;
                    break;
                }

                nextState = IDLE;
                break;
            }
            // en1_a is 1, en1_b is 0 
            case EN_CCW1: {
                if (b) {
                    nextState = EN_CCW2;
                    break;
                }

                if (!a) {
                    nextState = IDLE;
                    break;
                }

                nextState = EN_CCW1;
                break;
            }
            // en1_a, en1_b are 1
            case EN_CCW2: {
                if (!a) {
                    nextState = EN_CCW3;
                    break;
                }

                if (!b) {
                    nextState = EN_CCW1;
                    break;
                }

                nextState = EN_CCW2;
                break;
            }
            // en1_a is 0, en1_b is 1
            case EN_CCW3: {
                if (!b) {
                    nextState = EN_CCW_DONE;
                    break;
                }

                if (a) {
                    nextState = EN_CCW2;
                    break;
                }

                nextState = EN_CCW3;
                break;
            }
            // full cw rotation of en1
            // en1_a is 0, en1_b is 1
            case EN_CW1: {
                if (a) {
                    nextState = EN_CW2;
                    break;
                }

                if (!b) {
                    nextState = IDLE;
                    break;
                }

                nextState = EN_CW1;
                break;
            }
            // en1_b, en1_a are 1
            case EN_CW2: {
                if (!b) {
                    nextState = EN_CW3;
                    break;
                }

                if (!a) {
                    nextState = EN_CW1;
                    break;
                }

                nextState = EN_CW2;
                break;
            }
            // en1_a is 1, en1_b is 0
            case EN_CW3: {
                if (!a) {
                    nextState = EN_CW_DONE;
                    break;
                }

                if (b) {
                    nextState = EN_CW2;
                    break;
                }

                nextState = EN_CW3;
                break;
            }
            case EN_CW_DONE: {
                return ENCODER_CW;
            }
            case EN_CCW_DONE: {
                return ENCODER_CCW;
            }
            case EN_BTN_PRESSED: {
                return ENCODER1_BTN_PRESSED;
            }
            case EN2_BTN_PRESSED: {
                return ENCODER2_BTN_PRESSED;
            }
            case CLR_BTN: {
                return CLEAR_BTN;
            }
        }
    }
}
