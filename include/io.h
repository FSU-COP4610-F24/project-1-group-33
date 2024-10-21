#pragma once

#include "lever.h"
tokenlist* io_request(tokenlist *tokens);

bool io_open();

int io_amount(tokenlist *tokens);

int io_first_index(tokenlist *tokens);

void io_outputOpen(char* filename);

void io_outputClose();

void io_inputOpen(char* filename);

void io_inputClose();

void io_close();

void io_initialize();

tokenlist* io_cleanToken(tokenlist *tokens, int firstCaret);

extern int initialized;