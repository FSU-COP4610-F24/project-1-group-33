#include "Ev.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void expand_variable(char **token) {
    if((*token)[0] == '$') {
        char* envName = *token + 1;
        char* envValue = getenv(envName);

        if(envValue != NULL) {
            free(*token);
            *token = malloc(strlen(envValue)+ 1);

            strcpy(*token, envValue);
        }
    }
}

