#include "prompt.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int promptblock = 0; // Used by fork routing to display prompt at the proper time.

void blockPrompt(){
    promptblock = 1;
}

void display_prompt(){
if (promptblock == 0)
{
    char *user = getenv("USER");
    char *machine = getenv("HOSTNAME");                 //Accesses environment to create prompt

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
    
    printf("%s@%s:%s>", user, machine, cwd);
}
else
    {
    promptblock = 0;
    usleep (50000);
    display_prompt();
    }

}


