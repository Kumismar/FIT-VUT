#ifndef fsm_h
#define fsm_h

#define EN_CW 0
#define EN_CCW 1
#define EN_BTN 2
#define EN2_BTN 3

#define CHOOSE_OPERAND_OR_OPERATOR 1
#define PICK_FROM_PREVIOUS 2

/**
 * @brief States of the reading FSM.
 * Needed for keeping state of the rotary encoder when rotating the button, has 4 stages.
 */
typedef enum states {
    IDLE,
    EN_CW1, EN_CW2, EN_CW3, EN_CW_DONE,
    EN_CCW1, EN_CCW2, EN_CCW3, EN_CCW_DONE,
    EN_BTN_PRESSED, EN2_BTN_PRESSED,
    CLR_BTN,
} State;

/**
 * @brief Enum of every user interaction with the board.
 * Two rotary encoders and one button are used.
 */
typedef enum boardInteractions {
    ENCODER1_BTN_PRESSED, ENCODER2_BTN_PRESSED, 
    ENCODER_CW, ENCODER_CCW, 
    CLEAR_BTN
} BoardInteraction;

/**
 * @brief Actively wait for user interaction with board.
 * Changes functionality on chosen mode.
 * On CHOOSE_OPERAND_OR_OPERATOR, first encoder can be fully used along with button from second encoder.
 * On PICK_FROM_PREVIOUS only second encoder can be used.
 * On both, clear button is available.
 * 
 * @param mode Either CHOOSE_OPERAND_OR_OPERATOR or PICK_FROM_PREVIOUS.
 * @return int 
 */
BoardInteraction ReadBoardInteraction(int mode);

#endif