#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "prompt.h"
#include "Ev.h"
#include "lever.h"

char *get_input(void) {
    char *buffer = NULL;
    int bufsize = 0;
    char line[5];
    while (fgets(line, 5, stdin) != NULL)
    {
        int addby = 0;
        char *newln = strchr(line, '\n');
        if (newln != NULL)
            addby = newln - line;
        else
            addby = 5 - 1;
        buffer = (char *)realloc(buffer, bufsize + addby);
        memcpy(&buffer[bufsize], line, addby);
        bufsize += addby;
        if (newln != NULL)
            break;
    }
    buffer = (char *)realloc(buffer, bufsize + 1);
    buffer[bufsize] = 0;
    return buffer;
}

tokenlist *new_tokenlist(void) {
    tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
    tokens->size = 0;
    tokens->items = (char **)malloc(sizeof(char *));
    tokens->items[0] = NULL; /* make NULL terminated */
    return tokens;
}

void add_token(tokenlist *tokens, char *item) {
    int i = tokens->size;

    tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
    tokens->items[i] = (char *)malloc(strlen(item) + 1);
    tokens->items[i + 1] = NULL;
    strcpy(tokens->items[i], item);

    tokens->size += 1;
}

tokenlist *get_tokens(char *input) {
    char *buf = (char *)malloc(strlen(input) + 1);
    strcpy(buf, input);
    tokenlist *tokens = new_tokenlist();
    char *tok = strtok(buf, " ");
    while (tok != NULL)
    {
        add_token(tokens, tok);
        tok = strtok(NULL, " ");
    }
    free(buf);
    return tokens;
}

void free_tokens(tokenlist *tokens) {
    for (int i = 0; i < tokens->size; i++)
        free(tokens->items[i]);
    free(tokens->items);
    free(tokens);
}

void execute_commands(tokenlist *tokens) {    //executes commands
    if (tokens->size == 0) {                //checks if tokenlist is empty
        return;
    }
 
//    if(strcmp(tokens->items[0], "echo") == 0) {     //handles the echo command
//        for (int i = 1; i < tokens->size; i++) {
//            printf("%s\n", tokens->items[i]);   
//        }
//    }
        search(&tokens->items[0]);              //searchs for path for command
        int status;                             //
        pid_t pid = fork();                     //creates process
        
        if (pid == 0){
                execv(tokens->items[0], tokens->items); //replaces process with new command
                perror("execv");                        //
                exit(EXIT_FAILURE);                     //exit if fail
        }
        else if(pid > 0){
                waitpid(pid, &status, 0);               //wait for process to complete 
        }
        else{
                perror("fork");                         //incase fork fails   
        }
}


int main (){

    while(1){
        display_prompt();           //displays current directory
        char* input = get_input();
        tokenlist *tokens = get_tokens(input);

        for(int i = 0; i < tokens->size; i++) {
            expand_tilde(&(tokens->items[i]));
            expand_variable(&(tokens->items[i]));
        }
        execute_commands(tokens);

        free(input);
        free_tokens(tokens);
    }
}
