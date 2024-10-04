#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>  
#include <limits.h> 
#include <stdbool.h>
#include "path.h"

void search(char **token) {
    char *p = getenv("PATH"); //gets entire PATH   
    if (p == NULL) {
        printf("PATH environment variable not found.\n");
        return;
    }

    p = strdup(p);              //Deep copy

    bool pathExists = false;    //set to false until path is found  
    char *dir = strtok(p, ":");  //seperates paths with ':'

    while (dir != NULL) {                               
        size_t len = strlen(dir) + strlen(*token) + 2;  //allocates space for path
        char *path = malloc(len);                       //where full path goes

        if ((*token)[0] != '/') {
            snprintf(path, len, "%s/%s", dir, *token);  //adds '/'
        } else {
            snprintf(path, len, "%s%s", dir, *token);   //already has '/'
        }

                                                
        if (open(path, O_RDONLY) >= 0) {        //checks to see if file exist and readable
            pathExists = true;                          //path is found
            free(*token);                       //free space
            *token = malloc(strlen(path) + 1);          //allocates space for new token
            strcpy(*token, path);                       //copy new path to token
        }

        free(path);                                     //free space
        dir = strtok(NULL, ":");                        //get next dir if not found
    }

    free(p);                                            //free space

    if (!pathExists) {                          //unable to find path
        printf("Command Not Found\n");  
        }
}
