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



typedef enum {
    inputerror_noerror = 0,
    inputerror_noinput,
    inputerror_toolong
} InputError;

// Create a string token
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

// Print token values based on type
void token_print(Token* token) {
    if (token->type == tokentype_number) {
        printf("NumberToken: %f\n", ((NumberToken*) token)->value);
    } else if (token->type == tokentype_plus) {
    }
}

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


    // Free allocated memory
    free(stringtoken);
    return 0;
}
