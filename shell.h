#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "debug.h"

#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS 10 /* The maximum number of arguments */

#define ASC_NEWLINE 0x0a
#define ASC_BACKSLASH 0x5c

#define MAX_JOBS 10
#define MAX_JOB_NAME 5
#define MAX_JOB_STATUS 10

#define MAX_CHLD MAX_ARGS


typedef struct command_and_args {
    char **cmd; // head of command line
    int a_size; // how many args belong to the command
    char **in;
    char **out;
    char **err;
    struct command_and_args *next;
} command_t;

typedef struct job {
    int pid;
    char cmd[MAX_JOB_NAME]; // name of command
    char status[MAX_JOB_STATUS]; // status of job
} job_t;

/* global variables */
extern int shell_pid; // for signal handler
extern int child_pid; // for signal handler
extern job_t jobs[MAX_JOBS];

/* tokenize.c */
void shell_handler();
char *read_line();
void tokenize_line(char **args, char *line);

/* parse.c */
void parse_args(char **args, command_t *command);

/* prompt.c */
void print_user(void);
void init_shell(void);
void exit_shell(void);

/* builtin.c */
void builtin_cd(char *path);
void builtin_pwd(void);
void builtin_echo(char *str);
void builtin_bg(char *arg);
void builtin_fg(char *arg);

void builtin_jobs(void);
void init_jobs(void);
void add_job(int pid, char *cmd, char *status);

/* execute.c */
void command_handler(char *args[]);
void execute(char **args);

/* signalhandler.c */
void mask(int sig);
void unmask(int sig);
void sigint_handler(int sig);
void change_sigint_handler(void);
void sigtstp_handler(int sig);
void change_sigtstp_handler(void);

#endif /* _SHELL_H */
