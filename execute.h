#ifndef _execute_h
#define _execute_h

#include "shell.h"

#define CMD_LEN 100


/* execute.c */
void execve_search_path(char *args[]);
static void execute(char *cmd, char *args[]);
void command_handler(char *args[]);

#endif