#include <sys/wait.h>
#include <unistd.h>
#include "background.h"

int currentJobs = 0;
int totalJobs = 0;
int initializedFork = 0;
char* firstList;
char* secondList;
char* thirdList;
int records = 0;

    struct process forkJob(tokenlist *tokens){
    if (initializedFork == 0)
    {
        initializedFork = 1;                    // Main function of background processing.
        totalJobs = 0;                          // Copies tokenlist over to a standalone struct, ret
        currentJobs = 0;                        
    };
    while (currentJobs >= 10)
        sleep(1);

    currentJobs = currentJobs + 1;    
    totalJobs = totalJobs + 1;
    struct process ret;
    int dummy = fork();             

    ret.child = 1;
    int x = tokens->size - 1;
    ret.commandString = background_cleanToken(tokens,x);
    ret.commandLine = get_tokens(ret.commandString);
    int currentJobs = currentJobs + 1;
    ret.jobNum = totalJobs;
    ret.pid = getpid();
    if (dummy != 0){
        borrowFork(dummy,ret.jobNum,ret.commandString);
        struct process dummy;
        dummy.child = 0;
        return dummy;                                           //Parent process sends ret off to be catalogued in borrowFork, then hurries back to main() through a blank dummy.
        }
    else
        return ret;                         // Child process returns as the actual ret, which will then be executed in the background

    return ret;
    };

    void initializeForks(){
        initializedFork = 1;
        for (int i = 0; i < 10; i++){
            printf("%d",tenList[i].child);  // A side function to visualize the open slots in our 10 available processes, currently disabled.
        }
        printf("\n");
    }

    void borrowFork(int pid, int jobNum, char* commandString)
    {
    
    int indicator = -1;
    while (indicator == -1){
        for (int i = 0; i < 10; i++){
            if (tenList[i].child == 0)              // Checks processes until open one is found
            {
                indicator = i;
                i = 10;
            }
        }


    }

    tenList[indicator].child = 1;
    tenList[indicator].pid = pid;
    tenList[indicator].jobNum = jobNum;                                      //Sets all of the aspects of selected process to ret, allowing it to be reported and tracked
    tenList[indicator].commandString = strdup(commandString);
    tenList[indicator].listSlot = 0;

    }
    void processList(){
        int order = 1;
        for (int i = 0; i < 10; i++){
            if(tenList[i].child != 0)
            {
                int x;
                waitpid(tenList[i].pid, &x, WNOHANG);
                if ((getpgid(tenList[i].pid) >= 0)){
                    tenList[i].listSlot = tenList[i].listSlot + 1;          //One of the most complicated functions -- Checks which processes are running
                }                                                           // Running processes are given a +1 to listSlot per check, allowing for the 
                else                                                        // Differentiation between new, old, and closed processes.
                    tenList[i].listSlot = -1;
            };    
        }

        for (int i =0; i < 10; i++){

            if(tenList[i].listSlot < 1 && tenList[i].child == 1){
                printf("[%d] + %d done ",tenList[i].jobNum,tenList[i].pid);          // Marks the done and new processes, as in example
                printf(tenList[i].commandString);
                printf("\n");
            }
            
            if(tenList[i].listSlot == 1 && tenList[i].child == 1){
                printf("[%d] %d\n",tenList[i].jobNum,tenList[i].pid);
            }

        } 

        for (int i = 0; i < 10; i++){
            if(tenList[i].listSlot < 1 && tenList[i].child == 1){               // Anything marked as closed is finally cleaned up here, after being printed one last time
                tenList[i].child = 0;
                tenList[i].pid = 0;
                tenList[i].jobNum = 0;
                tenList[i].commandString = "";
                tenList[i].listSlot = 0;
                currentJobs = currentJobs - 1;
            }

        }
    }

void jobs(){
        for (int i = 0; i < 10; i++)                                            // Exact same as above, but only looks as running processes
        {
                if(tenList[i].listSlot >= 1 && tenList[i].child == 1){
                printf("[%d] + %d running ",tenList[i].jobNum,tenList[i].pid);
                printf(tenList[i].commandString);
                printf("\n");
                };
        };
}
    



char* background_cleanToken(tokenlist *tokenInput, int firstAnd)                // Same as io_cleanToken, removes & from tokens and returns
{
char* string = tokenInput->items[0];
for (int i = 1; i < firstAnd; i++)
{    
char* dummy = tokenInput->items[i];
string = strcat(string," ");
string = strcat(string,dummy);
};
tokenlist * ret = get_tokens(string);
return string;
}

void exitProgram(){
        if (records > 3)                                    //Exit program, exiting is actually done right after this, in the executeFunction.
            records = 3;
    printf("Last (%d) valid commands:\n",records);
    for (int i = 0; i < records; i++){
        printf("[%d]: ",i+1);
        if(i == 0)
            printf(firstList);
        if(i == 1)
            printf(secondList);
        if(i == 2)
            printf(thirdList);
        printf("\n");
    }
}

void depositExit(char * deposit){                   //ran everytime a correct input is made, uses a series of locks to progress records int, as it was refusing to be incremented normally.
    if (records > 2)
        thirdList = strdup(secondList);
    if (records > 1)
    {
        secondList = strdup(firstList);
        records = 3;
    };
    firstList = strdup(deposit);
    if (records <= 0)
        records = 2;

}