#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>  // For malloc and free



//
//
// ** Lexer
//
//

typedef enum {
    // Number
    tokentype_number = 0,
    
    // Operators
    tokentype_plus = 100,
    tokentype_minus = 101,
    tokentype_star = 102,
    tokentype_slash = 103,
    
    // Parenthesis
    tokentype_lparen = 200,
    tokentype_rparen = 201
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

typedef struct {
    Token token;
} ParenToken;



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

// Create an operator token
int parentoken_create(uint8_t pos, char value, ParenToken** out) {
    Token token;
    
    switch (value) {
        case '(':
            token.type = tokentype_lparen;
            break;
        case ')':
            token.type = tokentype_rparen;
            break;
        default:
            return 1;

    }
    token.pos = pos;

    ParenToken* parentoken = malloc(sizeof(ParenToken));
    if (parentoken == NULL) {
        return 2;  // Allocation error
    }

    parentoken->token = token;

    *out = parentoken;
    return 0;
}

// Print token values based on type
void token_print(Token* token) {
    if (token->type == tokentype_number) {
        printf("NumberToken: %f\n", ((NumberToken*) token)->value);
    } else if (token->type == tokentype_plus || token->type == tokentype_minus || token->type == tokentype_star || token->type == tokentype_slash) {
        printf("OperatorToken: %d\n", ((OperatorToken*) token)->token.type);
    } else if (token->type == tokentype_lparen || token->type == tokentype_rparen) {
        printf("ParenToken: %d\n", ((ParenToken*) token)->token.type);
    }
}

int lexer(char* input, void** tokens, size_t* token_amount) {

    *tokens = malloc(sizeof(void*) * 64);
    *token_amount = 0;

    size_t i = 0;
    char c;
    while(input[i] != '\0') {
        c = input[i];
        switch (c) {

            case '+':
            case '-':
            case '*':
            case '\\':
            case '/':

                OperatorToken* operatortoken;
                
                if (operatortoken_create(i, c, &operatortoken)) {
                    printf("Error creating operator token\n");
                    return 1;
                }

                tokens[*token_amount] = operatortoken;
                
                *token_amount = *token_amount + 1;

                break;

            case '(':
            case ')':

                ParenToken* parentoken;
                
                if (parentoken_create(i, c, &parentoken)) {
                    printf("Error creating paren token\n");
                    return 1;
                }

                tokens[*token_amount] = parentoken;
                
                *token_amount = *token_amount + 1;
                break;
        
        }

    }
    
    return 0;
}

//
//
// ** Parser
//
//

int parse() {
    return 1;
}



//
//
// ** Input
//
//

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


    ParenToken* lparentoken;
    if (parentoken_create(2, '(', &lparentoken)) {
        printf("Error creating lparen token\n");
        return 1;
    }
    token_print(&lparentoken->token);




    // Free allocated memory
    free(stringtoken);
    free(operatortoken);
    free(lparentoken);
    return 0;
}
