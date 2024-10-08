#include "internal.h"

void cd_command(tokenlist *tokens){
        if(tokens->size > 2){   //checks for 2 or more arguments                                        // Self explanatory, standalone implementation of how cd works.
                printf("Too many arguments\n");
        }
        else if(tokens->size ==1){   //when it finds only on argument
                char* home = getenv("HOME"); //get home env
                if(home!= NULL){  //checks for NULL
                        if(chdir(home) != 0)
                                perror("failure with cd");
                }
        }
        else{      // checks to see if the file/directory exist
                if(chdir(tokens->items[1]) != 0){
                        perror("file does no exist");
                }

        }
        return;
}
