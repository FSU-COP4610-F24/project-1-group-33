#include "Ev.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void expand_variable(char **token) { //checks for $ & passed as double pointer to modify the original pointer
    if((*token)[0] == '$') {
        char* envName = *token + 1;      //ignores $
        char* envValue = getenv(envName);      //gets Environment Variable

        if(envValue != NULL) {              //checks to see if the Environment Variable exist
            free(*token);
            *token = malloc(strlen(envValue)+ 1);    //allocates space for +1 for the /0

            strcpy(*token, envValue);
        }
    }
