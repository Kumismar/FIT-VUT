#include "headers/fsm.h"

#ifdef CBMC_VERIFICATION
/* === MOCK HARDWARE PRO CBMC === */
#    include <stdint.h>
int nondet_int();

typedef struct
{
    uint32_t PDIR;
} GPIO_Type;

GPIO_Type mock_pta;
GPIO_Type mock_pte;

GPIO_Type* PTA = &mock_pta;
GPIO_Type* PTE = &mock_pte;

#    define FSM_LOOP_LIMIT 200
#else
#    include "MK60D10.h"
#endif

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

#ifdef CBMC_VERIFICATION
    // Pro verifikaci bezime jen omezeny pocet kroku
    for (int iter = 0; iter < FSM_LOOP_LIMIT; iter++) {
        PTA->PDIR = nondet_int();
        PTE->PDIR = nondet_int();
#else
    // Puvodni nekonecna smycka pro mikrokontroler
    while (1) {
#endif
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
#ifdef CBMC_VERIFICATION
    return -1;
#endif
}

/* ========================================================= */
/* VERIFIKACNI HARNESS                                       */
/* ========================================================= */
#ifdef CBMC_VERIFICATION
#    include <assert.h>

int main()
{
    // Spustime FSM. CBMC prozkouma vsechny mozne kombinace vstupu (nondet_int)
    // a vsechny cesty stavovym automatem az do hloubky FSM_LOOP_LIMIT.

    // Volame s rezimem 1 (CHOOSE_OPERAND_OR_OPERATOR)
    int result = ReadBoardInteraction(CHOOSE_OPERAND_OR_OPERATOR);

    // Pokud funkce vratila vysledek (nedoslo k timeoutu smycky),
    // musi to byt platna hodnota z enumu.
    if (result != -1) {
        assert(result == ENCODER1_BTN_PRESSED || result == ENCODER2_BTN_PRESSED ||
               result == ENCODER_CW || result == ENCODER_CCW || result == CLEAR_BTN);

        // Muzeme zkusit i silnejsi tvrzeni:
        // Existuje cesta, ktera vede k ENCODER_CW?
        // (Toto by normalne vyzadovalo 'cover' property, ale assert(result != ENCODER_CW)
        // by nam vyhodil protipriklad = uspesny pruchod, pokud je to mozne).
    }

    return 0;
}
#endif