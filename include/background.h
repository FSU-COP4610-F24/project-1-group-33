#pragma once
#include <sys/wait.h>
#include "lever.h"

struct process {
    int pid;
    int jobNum;
    tokenlist* commandLine;
    char * commandString;
    int child;
    int listSlot;
};

struct process forkJob(tokenlist *tokens);
extern struct process tenList[11];

void holdUntilDone();
void startProcess();
char *background_cleanToken(tokenlist* tokens,int firstAnd);
void depositFork(struct process newFork);
void initializeForks();
void borrowFork(int pid, int jobNum, char* commandString);