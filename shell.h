#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "type.h"
#include "job.h"
#include "command_util.h"
#include "execute.h"
#include "debug.h"

#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS 10 /* The maximum number of arguments */

#define ASC_NEWLINE 0x0a
#define ASC_BACKSLASH 0x5c
#define ASC_PIPE 0x7c

#define MAX_CHLD MAX_ARGS

/* tokenize.c */
void shell_handler();
char *read_line();
void tokenize_line(char **args, char *line);

/* prompt.c */
void print_user(void);
void init_shell(void);
void exit_shell(void);

/* builtin.c */
void builtin_cd(char *path);
void builtin_pwd(void);
void builtin_echo(char *str);

/* signalhandler.c */
void mask(int sig);
void unmask(int sig);
void sigint_handler(int sig);
void change_sigint_handler(void);
void sigtstp_handler(int sig);
void change_sigtstp_handler(void);

#endif /* _SHELL_H */
