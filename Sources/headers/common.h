#ifndef common_h
#define common_h

#define LEN 100

#define BEEP_MASK (1 << 4)
#define EN2 300
#define EN1 150
#define CW 1500
#define BTN 1000
#define CCW 500
#define CLR 750

/**
 * @brief Data structure holding operand or result value.
 */
typedef union value {
    int intResult;
    float floatResult;
} Value;

/**
 * @brief Data structure holding information about operand or result data type.
 * FAULT when computing operation fails.
 */
typedef enum type {
    INT, FLOAT, FAULT
} Type;

/**
 * @brief Data structure with type of error after last computing operation.
 */
typedef enum faultType {
    NONE, DIVISION_ZERO, SQRT_LT_ZERO, NAN_FAULT, INF_FAULT
} FaultType;

/**
 * @brief Data structure holding information about operands and results.
 */
typedef struct calculatorData {
    Type type;
    Value value;
    FaultType faultType;
} CalcData;

/**
 * @brief Execute 'length' NOP instructions (actively block mcu from further usage).
 * 
 * @param length 
 */
void delay(int length);

/**
 * @brief Generate tone on PIEZZO speaker.
 * Generate rectangle signal with 2x'depth' period (executes 'depth' NOP instructions after setting 1 or 0 to GPIO speaker output).
 * 
 * @param length Length of the tone.
 * @param depth Depth of the tone.
 */
void beep(int length, int depth);

#endif
