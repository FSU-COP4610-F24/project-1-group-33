#include "prompt.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>


void display_prompt(){
    char *user = getenv("USER");
    char *machine = getenv("HOSTNAME");

    char cwd[PATH_MAX];

    if(getcwd(cwd, sizeof(cwd)) == NULL){
        char* home = getenv("HOME");
        if(home != NULL){
            strcpy(cwd, home);
        }
        else{
            perror("getcwd() error");
            return;
        }
    }

    printf("%s@%s:%s> ", user, machine, cwd);
}
