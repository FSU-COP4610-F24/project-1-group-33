#pragma once
#include "io.h"
#include <stdio.h>
#include "fcntl.h"
#include <unistd.h>
#include <string.h>



int in = 0;
int in2 = 0;
int out = 0;
int out2 = 0;
bool inSuccess = 0;

tokenlist* io_request(tokenlist *tokens){
if (initialized == 0){
    io_initialize();
}

int a = io_amount(tokens);
int b = io_first_index(tokens);                         // Main function of io, takes input and logically decides which order the tokens are in.

    
if(a == 1)
{
    if(strcmp(tokens->items[b], "<") == 0){
        io_inputOpen(tokens->items[b + 1]);
        tokenlist * dummy = io_cleanToken(tokens,b);
        return (dummy);
    }

    if(strcmp(tokens->items[b], ">") == 0){
        io_outputOpen(tokens->items[b + 1]);
        tokenlist * dummy = io_cleanToken(tokens,b);
        return(dummy);
        }
}
if (a == 2)
{
    if(strcmp(tokens->items[b], "<") == 0){
        io_outputOpen(tokens->items[b + 3]);
        io_inputOpen(tokens->items[b + 1]);
        
        tokenlist * dummy = io_cleanToken(tokens,b);
        return(dummy);
    }



    if(strcmp(tokens->items[b], ">") == 0 && strcmp(tokens->items[3], "<") == 0){
        io_outputOpen(tokens->items[b + 1]);
        io_inputOpen(tokens->items[b + 3]);
        tokenlist * dummy = io_cleanToken(tokens,b);
        return(dummy);
    }
}
return tokens;
}

/*bool io_open(){
 int mode = out;
    if (mode > 0){
        return true;
    }
    else                        // Returns true if file is open
    {   
        return false;
    }; 
} */


int io_amount(tokenlist *tokens)
{
    int x = tokens->size;
    int carret = 0;
    for (int i = 0; i < x; i++)                         // Used to return how many angle brackets are in a tokenlist ( Decides whether i/o or i&o)
    {

        if(strcmp(tokens->items[i], ">") == 0 || (strcmp(tokens->items[i], "<") == 0))
        {
        carret++;
        };    
    }

return carret;

}

int io_first_index(tokenlist *tokens)
{                                                     //Similiar to amount, returns the index where the first angle bracket is. Makes this all easy.
    int x = tokens->size;
    int index = 0;
    for (int i = 0; i < x; i++)
    {
        if(strcmp(tokens->items[i], ">") == 0 || (strcmp(tokens->items[i], "<") == 0))
        {
        index = i;
        i = x;
        };
    }
return index;


}

void io_initialize(){
dup2(0,4);          //stdIn to 4
dup2(1,5);          //stdOut to 5                               // Keeps copies of stdin & stdout to the side, whether they cannot be closed
initialized = 1;
};

void io_outputOpen(char* filename){
int tempOut = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);  // Assigns to 3 
close(1);
dup2(tempOut, 1);           // Places 3 in 1
close(tempOut);                   // Cleans up 3, so that next io assignment will wind up 3
tempOut = -2;
};

void io_outputClose(){
close(1);                   // Closes open file and replaces it with the stored stdOut
dup2(5,1);
};

void io_inputOpen(char* filename){
int tempIn = open(filename, O_RDONLY);  // Assigns to 3
if (tempIn < 0)
{
    printf("Error: Bad File Name");
    inSuccess = 0;
}
else{
close(0);
dup2(tempIn,0);                  // Places 3 in 0
close(tempIn);                   // Cleans up 3, so that next io assignment will wind up 3
tempIn = -2;
};
};

void io_inputClose(){
close(0);                   // Closes in file and replaces it with the stored stdIn
dup2(4,0);
};

void io_close(){
    io_inputClose();
    io_outputClose();
}

tokenlist* io_cleanToken(tokenlist *tokenInput, int firstCaret)
{
char* string = tokenInput->items[0];
for (int i = 1; i < firstCaret; i++)
{    
char* dummy = tokenInput->items[i];                 // Cleans everything out of tokenlist except for cmd, so that it can be rerouted back into executeCommands with new i/o
string = strcat(string," ");
string = strcat(string,dummy);
};
tokenlist * ret = get_tokens(string);
return ret;
}