#include <stdlib.h>
#include <string.h>

char * expand_tilde(char ** token) {
    if(*token[0] == '~') {
        char* home = getenv("HOME");
        if(home != NULL) {
            size_t len = strlen(home) + strlen(*token) - 1; //gets rid of ~
            char *expanded = (char *)malloc (len + 1); //allocates space

            if (strcpy(*token, "~") == 0) {     //checks for only ~
                strcpy(expanded, home);
            }
            else {
                strcpy(expanded, home); // adds homepath + token
                strcat(expanded, *token + 1);
            }
            free(*token);
            *token = expanded;
        }
    }
}

