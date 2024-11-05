#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>  // For malloc and free

typedef enum {
    // Number
    tokentype_number,
    
    // Operators
    tokentype_plus,
    tokentype_minus,
    tokentype_star,
    tokentype_slash,
    
    // Parenthesis
    tokentype_lparen,
    tokentype_rparen
} TokenType;

typedef struct {
    TokenType type;
    uint8_t pos;
} Token;

typedef struct {
    Token token;
    double_t value;
} NumberToken;


typedef struct {
    Token token;
} OperatorToken;



// Create a number token
int numbertoken_create(uint8_t pos, double_t value, NumberToken** out) {
    Token token;
    token.type = tokentype_number;
    token.pos = pos;

    NumberToken* numbertoken = malloc(sizeof(NumberToken));
    if (numbertoken == NULL) {
        return 1;  // Allocation error
    }

    numbertoken->token = token;
    numbertoken->value = value;

    *out = numbertoken;
    return 0;
}

// Create an operator token
int operatortoken_create(uint8_t pos, char value, OperatorToken** out) {
    Token token;
    
    switch (value) {
        case '+':
            token.type = tokentype_plus;
            break;
        case '-':
            token.type = tokentype_minus;
            break;
        case '*':
            token.type = tokentype_star;
            break;
        case '\\':
        case '/':
            token.type = tokentype_slash;
            break;
        default:
            return 1;

    }
    token.pos = pos;

    OperatorToken* operatortoken = malloc(sizeof(OperatorToken));
    if (operatortoken == NULL) {
        return 2;  // Allocation error
    }

    operatortoken->token = token;

    *out = operatortoken;
    return 0;
}

// Print token values based on type
void token_print(Token* token) {
    if (token->type == tokentype_number) {
        printf("NumberToken: %f\n", ((NumberToken*) token)->value);
    } else if (token->type == tokentype_plus || token->type == tokentype_minus || token->type == tokentype_star || token->type == tokentype_slash) {
        printf("OperatorToken: %d\n", ((OperatorToken*) token)->token.type);
    }
}


typedef enum {
    inputerror_noerror = 0,
    inputerror_noinput,
    inputerror_toolong
} InputError;

// Get user input with buffer overflow protection
static InputError getInputLine(char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    if (prmpt != NULL) {
        printf("%s", prmpt);
        fflush(stdout);
    }
    if (fgets(buff, sz, stdin) == NULL)
        return inputerror_noinput;

    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? inputerror_toolong : inputerror_noerror;
    }

    buff[strlen(buff)-1] = '\0';  // Remove newline character
    return inputerror_noerror;
}

int main(void) {
    InputError rc;
    char buff[128];

    rc = getInputLine("> ", buff, sizeof(buff));  // Get user input
    if (rc == inputerror_noinput) {
        printf("\nNo input\n");
        return 1;
    }

    if (rc == inputerror_toolong) {
        printf("Input too long [%s]\n", buff);
        return 1;
    }

    printf("OK [%s]\n", buff);  // Print user input

    NumberToken* stringtoken;
    if (numbertoken_create(2, 25, &stringtoken)) {
        printf("Error creating number token\n");
        return 1;
    }
    token_print(&stringtoken->token);

    OperatorToken* operatortoken;
    if (operatortoken_create(2, '+', &operatortoken)) {
        printf("Error creating operator token\n");
        return 1;
    }
    token_print(&operatortoken->token);


    // Free allocated memory
    free(stringtoken);
    free(operatortoken);
    return 0;
}
