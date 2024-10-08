#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "prompt.h"
#include "Ev.h"
#include "lever.h"
#include "tilde.h"
#include "path.h"
#include "internal.h"
#include "io.c"
#include "pipe.h"
#include "background.c"


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

tokenlist *new_tokenlist(void) {                //Holds tokens in array, dynamic. Used in most functions.
    tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
    tokens->size = 0;
    tokens->items = (char **)malloc(sizeof(char *));
    tokens->items[0] = NULL;  //make NULL terminated 
    return tokens;
}

void add_token(tokenlist *tokens, char *item) {         // Generic restructor + add function for Tokenlist
    int i = tokens->size;

    tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
    tokens->items[i] = (char *)malloc(strlen(item) + 1);
    tokens->items[i + 1] = NULL;
    strcpy(tokens->items[i], item);

    tokens->size += 1;
}

tokenlist *get_tokens(char *input) {                    // Turns tokens into one string, primarily used for output
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

void free_tokens(tokenlist *tokens) {               // Cleans out tokenlist, for reuse on next input
    for (int i = 0; i < tokens->size; i++)
        free(tokens->items[i]);
    free(tokens->items);
    free(tokens);
}

void execute_commands(tokenlist *tokens) {    //executes commands
    if (tokens->size == 0) {                //checks if tokenlist is empty
        return;
    }

        char*tokenCopy = strdup(tokens->items[0]); 
        if(tokens->size > 1){
            for (int i = 1; i < tokens->size; i++)      // A duplicate of *tokens, to be funneled into the last 3 successful commands,
            {                                           // See inputSuccess & depositExit for actual use
                char*dum = strdup(tokens->items[i]);
                strcat(tokenCopy," ");
                strcat(tokenCopy,dum);
            };
        }

            

        if(strcmp(tokens->items[0], "cd") == 0){
                cd_command(tokens);
        }
        if(strcmp(tokens->items[0], "jobs") == 0){
                jobs();                                                 // 3 Main internal functions that were easier to add as exceptions leading to functions.
                return;
        }
        if(strcmp(tokens->items[0], "exit") == 0){
                exitProgram();
                exit(0);
        }

        if (tokens->size > 1){
            int x = tokens->size;
            int y = 0;
            for (int i = 0; i < x; i++)
            {
            if(strcmp(tokens->items[i], "&") == 0){                     // If more than 1 token, need to check for an & symbol.
                y++;                                        
            }
            };
            if(y > 0)
            {
                blockPrompt();
                struct process activeProcess = forkJob(tokens);             // If so, send off to background.c, where it will fork, shed the & symbol, and execute in the background 
                if (activeProcess.child == 1){
                    usleep (50000);                                                             
                    execute_commands(activeProcess.commandLine);
                    display_prompt();                                       //Fighting weirdness with prompt getting out of sync with background processes.
                    display_prompt();    
                    kill(activeProcess.pid, SIGTERM);
                }
                else
                {
                    return;
                };
            };
        };


        if (tokens->size > 1){
            int x = tokens->size;
            int y = 0;
            for (int i = 0; i < x; i++)
            {                                                                                   // Very similiar to &, but checks for io commands.
            if(strcmp(tokens->items[i], "<") == 0 || strcmp(tokens->items[i], ">") == 0){       // Will go in, open relevant streams, and then execute the commands.
                y++;                                                                            // Disable the streams after.
            }
            };
            if (y > 0){
                tokenlist * dummy = io_request(tokens);
                execute_commands(dummy);
                io_close();
                return;

                }
            }

	    for(int i = 0; i < tokens->size; i++){
		    if(strcmp(tokens->items[i], "|") == 0){                                         //Final of the checks, looking for potential piping.
			    execute_piping(tokens);
			    return;
		    }

	    }
        

        search(&tokens->items[0]);              //searchs for path for command
        int status;                             //
        pid_t pid = fork();                     //creates process
        if (inputSuccess() == 1)
            depositExit(tokenCopy);

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

    display_prompt();
    while(1){
        // if not coming from background
        char* input = get_input();
        tokenlist *tokens = get_tokens(input);

        for(int i = 0; i < tokens->size; i++) {
            expand_tilde(&(tokens->items[i]));
            expand_variable(&(tokens->items[i]));
        }
        
        execute_commands(tokens);

        free(input);
        free_tokens(tokens);
        usleep (40000);
        processList();
        display_prompt();
    }
};

