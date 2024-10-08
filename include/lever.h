#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct {                                //Some beginning of file starter code.
    char ** items;
    size_t size;
} tokenlist;

char * get_input(void);
tokenlist * get_tokens(char *input);
tokenlist * new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);

