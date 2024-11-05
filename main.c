#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>  // For malloc and free

typedef enum {
    tokentype_string,
    tokentype_int,
    tokentype_float
} TokenType;

typedef struct {
    TokenType type;
    char* name;
} Token;

typedef struct {
    Token token;
    char* value;
} StringToken;

typedef struct {
    Token token;
    int value;
} IntToken;

typedef enum {
    inputerror_noerror = 0,
    inputerror_noinput,
    inputerror_toolong
} InputError;

// Create a string token
int stringtoken_create(char* name, char* value, StringToken** out) {
    Token token;
    token.type = tokentype_string;
    token.name = name;

    StringToken* stringtoken = malloc(sizeof(StringToken));
    if (stringtoken == NULL) {
        return 1;  // Allocation error
    }

    stringtoken->token = token;
    stringtoken->value = value;

    *out = stringtoken;
    return 0;
}

// Create an integer token
int inttoken_create(char* name, int value, IntToken** out) {
    Token token;
    token.type = tokentype_int;
    token.name = name;

    IntToken* inttoken = malloc(sizeof(IntToken));
    if (inttoken == NULL) {
        return 1;  // Allocation error
    }

    inttoken->token = token;
    inttoken->value = value;

    *out = inttoken;
    return 0;
}

// Print token values based on type
void token_print(Token* token) {
    if (token->type == tokentype_string) {
        printf("StringToken: %s\n", ((StringToken*) token)->value);
    } else if (token->type == tokentype_int) {
        printf("IntToken: %d\n", ((IntToken*) token)->value);
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

    StringToken* stringtoken;
    if (stringtoken_create("input", buff, &stringtoken)) {
        printf("Error creating string token\n");
        return 1;
    }

    token_print(&stringtoken->token);

    IntToken* inttoken;
    if (inttoken_create("input", 60, &inttoken)) {
        printf("Error creating int token\n");
        return 1;
    }

    token_print(&inttoken->token);

    // Free allocated memory
    free(stringtoken);
    free(inttoken);
    return 0;
}
