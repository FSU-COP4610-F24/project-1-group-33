#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "path.h"
#include "lever.h"

void execute_piping(tokenlist *tokens);
