# Project 1 COP4610 (Shell)

## Group Members
- **Ian Spence**: ips22a@fsu.edu
- **Amisaday Santiago**: as21bj@fsu.edu
- **Austin Dennison**: amd23r@fsu.edu
  
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Output prompt with the absolute working directory, user name, and the machine name
- **Assigned to**: Amisaday, Ian

### Part 2: Environment Variables
- **Responsibilities**: Replace tokens prefixed with `$` with their corresponding values. These are environment variables.
- **Assigned to**: Amisaday

### Part 3: Tilde Expansion
- **Responsibilities**: `~` shall expand to environment variable `$HOME`.
- **Assigned to**: Austin

### Part 4: $PATH Search
- **Responsibilities**: Commands not built in and without a leading `/` must search the directories listed in `$PATH`.
- **Assigned to**: Ian

### Part 5: External Command Execution
- **Responsibilities**: `fork()` a child process. This process will then execute the desired command using the `execv()` function.
- **Assigned to**: Autin

### Part 6: I/O Redirection
- **Responsibilities**: Allows us to replace the keyboard with input from a specified file and redirect output to a designated file.
- **Assigned to**: Amisaday

### Part 7: Piping
- **Responsibilities**: Allows for output of one command to be redirected to another.
- **Assigned to**: Amisaday

### Part 8: Background Processing
- **Responsibilities**: Allows the shell to execute commands without waiting for their completion.
- **Assigned to**: Ian, Amisaday

### Part 9: Internal Command Execution
- **Responsibilities**: Create native shell commands.
- **Assigned to**: Ian, Amisaday, Austin

### Extra Credit Unlimited Piping
- **Responsibilities**: Expand the functionality of the shell to support an unlimited number of pipes in a single command. This allows for more complex command pipelines and advanced data processing.
- **Assigned to**: Amisaday

## File Listing
```
root/
  src/
    Ev.c
    internal.c
    main.c
    path.c
    pipe.c
    prompt.c
    tilde.c
  include/
    Ev.h
    internal.h
    lever.h
    path.h
    pipe.h
    prompt.h
    tilde.h
  README.md
  Makefile
```
## How to Compile & Execute

### Requirements
-gcc

### Compilation
-make

### Execution
-op

This will run the program

## Bugs
Notifications for ending background processes sometimes come before OR after a main call, depending on runtimes.

## Extra Credit
- **Extra Credit 1**: Support unlimited number of pipes [2]

## Considerations
Ian Spence's Late Work
