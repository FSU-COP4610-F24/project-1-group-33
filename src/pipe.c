#include "pipe.h"

void execute_piping(tokenlist *tokens) {
    int pipe_count = 0; 
    for (int i = 0; i < tokens->size; i++) { 
        if (strcmp(tokens->items[i], "|") == 0) {
            pipe_count++;
        }
    }

    int pipefds[2 * pipe_count]; 
    char **command_paths = malloc((pipe_count + 1) * sizeof(char *)); 
    

    for (int i = 0; i < pipe_count; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int cmd_start = 0; 
    int path_index = 0;

    for (int i = 0; i <= pipe_count; i++) {
        int cmd_end = cmd_start; 
        while (cmd_end < tokens->size && strcmp(tokens->items[cmd_end], "|") != 0) {
            cmd_end++; 
        }

        
        char **cmd = malloc((cmd_end - cmd_start + 1) * sizeof(char *));
        for (int j = cmd_start; j < cmd_end; j++) {
            cmd[j - cmd_start] = tokens->items[j];  
        }
        cmd[cmd_end - cmd_start] = NULL; 

       
        search(&cmd[0]); 
        command_paths[path_index] = strdup(cmd[0]);

        path_index++; 
        cmd_start = cmd_end + 1; 

        

    
        pid_t pid = fork(); 

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { 
          
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
  
            if (i < pipe_count) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * pipe_count; j++) {
                close(pipefds[j]);
            }

       
            execv(command_paths[i], cmd);  
            perror("execv");
            exit(EXIT_FAILURE);
        }

        free(cmd);
    }

 
    for (int i = 0; i < 2 * pipe_count; i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i <= pipe_count; i++) {
        wait(NULL);
    }

  
    for (int i = 0; i <= pipe_count; i++) {
        free(command_paths[i]);
    }
    free(command_paths);
}
