#ifndef _JOB_H_
#define _JOB_H_

#include "shell.h"

#define MAX_JOBS 10
#define MAX_JOB_NAME 5
#define MAX_JOB_STATUS 10

typedef struct job {
    int pid;
    char cmd[MAX_JOB_NAME]; // name of command
    char status[MAX_JOB_STATUS]; // status of job
} job_t;

/* global variables */
extern int shell_pid; // for signal handler
extern int child_pid; // for signal handler
extern job_t jobs[MAX_JOBS];


void builtin_bg(char *arg);
void builtin_fg(char *arg);

void builtin_jobs(void);
void init_jobs(void);
void add_job(int pid, char *cmd, char *status);


#endif