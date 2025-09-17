#include "headers/message_parsing.h"

// TODO: Tests for parser

int lexicalAnalysis(const char *buffer)
{
    int i = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
    {
        switch (buffer[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '(':
            case ')':
            case '+':
            case '-':
            case '*':
            case '/':
            case ' ':
            case '\t':
            {
                i++;
                continue;
            }
            default:
                return LEX_FAILED;
        }
    }
    return LEX_SUCCESS;
}

bool isValidOperator(char c)
{
    switch (c)
    {
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
        default:
            return false;
    }
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool isLeftPar(char c)
{
    return c == '(';
}

bool isRightPar(char c)
{
    return c == ')';
}

bool isWhiteSpace(char c)
{
    return (c == ' ' || c == '\t');
}

void doOperation(int op1, int op2, char operator, char *result)
{
    int numResult = 0;
    switch (operator)
    {
        case '+':
            numResult = op1 + op2;
            break;
        case '-':
            numResult = op1 - op2;
            break;
        case '*':
            numResult = op1 * op2;
            break;
        case '/': 
            numResult = op1 / op2;
            break;
        default:
            errorExit("switch do operation default", PROGRAM_FAILURE);
    }
    sprintf(result, "%d", numResult);
}

void evaluateExpression(Stack *exp, char *result)
{
    Stack eval;
    initStack(&eval);

    stackNode *tmp = exp->top; 

    while (tmp != NULL)
    {
        char c;
        sscanf(tmp->s, "%c", &c);
        if (isDigit(c))
            push(&eval, tmp->s);

        else if (isValidOperator(c))
        {
            int operand1;
            sscanf(top(&eval), "%d", &operand1);
            pop(&eval);

            int operand2;
            sscanf(top(&eval), "%d", &operand2);
            pop(&eval);

            doOperation(operand1, operand2, c, result);

            push(&eval, result);
        }
        tmp = tmp->next;
    }
    memset(result, 0, MAX_BUFFER_LENGTH);
    strcpy(result, top(&eval));
    strcat(result, "\n");
    disposeStack(&eval);
}

void getErrorMessageUDP(char *buffer, char *errorMessage)
{
    memset(buffer, 0, MAX_BUFFER_LENGTH);
    char *message = buffer + SKIP_OPCODE_STATUS_LENGTH;
    buffer[OPCODE] = 1;
    buffer[STATUS_CODE] = 1;
    buffer[MSGLENGTH] = (char)strlen(errorMessage) + OPCODE_STATUS_LENGTH;
    strcpy(message, errorMessage);
}

void getErrorMessageTCP(char *buffer, char *errorMessage)
{
    memset(buffer, 0, MAX_BUFFER_LENGTH);
    strcpy(buffer, errorMessage);
}

void getResponseMessageUDP(char *buffer, char *result)
{
    memset(buffer, 0, MAX_BUFFER_LENGTH);
    char *message = buffer + SKIP_OPCODE_STATUS_LENGTH;
    strcpy(message, result);
    buffer[OPCODE] = 1;
    buffer[STATUS_CODE] = 0;
    buffer[MSGLENGTH] = (char)strlen(message) + OPCODE_STATUS_LENGTH;
}

void getResponseMessageTCP(char *buffer, char *result)
{
    memset(buffer, 0, MAX_BUFFER_LENGTH);
    char resultMessage[] = "RESULT ";
    strcpy(buffer, resultMessage);
    strcpy(buffer + strlen(resultMessage), result);
}

void getNextToken(char *buffer, Token *token, bool newQuery)
{
    static int position = 0;

    /* Reset buffer position for every new query */
    if (newQuery == true)
        position = 0;

    memset(token->data, 0, MAX_BUFFER_LENGTH);

    /* Skipping all the whitespaces */
    if (isWhiteSpace(buffer[position]))
        while (isWhiteSpace(buffer[++position]));

    if (isDigit(buffer[position]))
    {
        /* Take all the numeric characters and insert them into token->data (they represent one number) */
        int i = 0;
        while (isDigit(buffer[position]))
        {
            token->data[i] = buffer[position];
            position++; i++;
        }
        token->type = number;
        return;
    }
    
    switch (buffer[position])
    {
        case '+':
        case '-':
        case '*':
        case '/':
            token->type = operatorToken;
            break;
        case '(':
            token->type = leftPar;
            break;
        case ')':
            token->type = rightPar;
            break;
        case '\n':
            token->type = newLine;
            break;
        default:
            /* Because we already check lexemes. */
            errorExit("Should never get here (getNextToken).\n", PROGRAM_FAILURE);
    }
    token->data[0] = buffer[position];
    position++;
}

int parseOperator(Token *token, Stack *stack)
{
    if (token->type != operatorToken)
        return PARSE_FAILED;

    push(stack, token->data);
    return PARSE_SUCCESS;
}

int parseExpression(char *buffer, Token *token, Stack *stack)
{
    if (token->type == number)
    {
        push(stack, token->data);
        return PARSE_SUCCESS;
    }

    if (token->type != leftPar)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseOperator(token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseExpression(buffer, token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseExpression(buffer, token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (token->type != rightPar)
        return PARSE_FAILED;

    return PARSE_SUCCESS;
}

int parseQuery(char *buffer, Token *token, Stack *stack)
{
    if (token->type != leftPar)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseOperator(token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseExpression(buffer, token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (parseExpression(buffer, token, stack) == PARSE_FAILED)
        return PARSE_FAILED;

    getNextToken(buffer, token, false);
    if (token->type != rightPar)
        return PARSE_FAILED;

    return PARSE_SUCCESS;
}

char* parseRecievedQuery(char *buffer, void (*getResponseMessage)(char*, char*), Modes mode)
{
    Token token = {0};
    char *result = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH);
    /* The number can theoretically be 256 digits long. */
    token.data = (char*)mallocAndAddToList(MAX_BUFFER_LENGTH); 

    Stack expStack;
    initStack(&expStack); 

    char* bufferQueryPosition;

    if (mode == TCP)
    {
        bufferQueryPosition = buffer + strlen("SOLVE ");
        if (strncmp(buffer, "SOLVE ", strlen("SOLVE ")) != 0)
        {
            getErrorMessageTCP(buffer, "BYE\n");
            return buffer;
        }

        if (lexicalAnalysis(bufferQueryPosition) == LEX_FAILED)
        {
            getErrorMessageTCP(buffer, "BYE\n");
            return buffer;
        }

        getNextToken(bufferQueryPosition, &token, true);
        if (parseQuery(bufferQueryPosition, &token, &expStack))
        {
            getErrorMessageTCP(buffer, "BYE\n");
            return buffer;
        }
    }
    else
    {
        bufferQueryPosition = buffer + SKIP_OPCODE_MSGLENGTH;
        if (lexicalAnalysis(bufferQueryPosition) == LEX_FAILED)
        {
            getErrorMessageUDP(buffer, "Lexical error: Invalid lexeme\n");
            return buffer;
        }

        getNextToken(bufferQueryPosition, &token, true);
        if (parseQuery(bufferQueryPosition, &token, &expStack) == PARSE_FAILED)
        {
            getErrorMessageUDP(buffer, "Syntax error: Invalid token\n");
            return buffer;
        }
    }

    evaluateExpression(&expStack, result);
    memset(buffer, 0, MAX_BUFFER_LENGTH);
    getResponseMessage(buffer, result);

    disposeStack(&expStack);
    freeAndRemoveFromList(token.data);
    return buffer;
}