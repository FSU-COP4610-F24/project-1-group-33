#include <sys/wait.h>

struct process {
    int pid;
    int jobNum;
    tokenlist* commandLine;
    char * commandString;
    int child;
    int listSlot;
};

void holdUntilDone();
void startProcess();
char *background_cleanToken(tokenlist* tokens,int firstAnd);
struct process tenList[11];
void depositFork(struct process newFork);
void initializeForks();
void borrowFork(int pid, int jobNum, char* commandString);