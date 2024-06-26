#include "job.h"

/** JOB HANDLER **/

/* jobs */
void builtin_jobs(void)
{
    int i;
    printf("Jobs:\n");
    for (i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d] %s %d %s\n", i + 1, jobs[i].cmd, jobs[i].pid, jobs[i].status);
        }
    }
}

void init_jobs(void)
{
    int i;
    for (i = 0; i < MAX_JOBS; i++) {
        jobs[i].pid = 0;
    }
}

void add_job(int pid, char *cmd, char *status)
{
    int i;
    for (i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == 0) {
            jobs[i].pid = pid;
            strcpy(jobs[i].cmd, cmd);
            strcpy(jobs[i].status, status);
            break;
        }
    }
    if (i == MAX_JOBS) {
        fprintf(stderr, "Too many jobs\n");
    }
}

/* bg */
void builtin_bg(char *arg)
{
    int job_id;
    if (arg == NULL) {
        job_id = 1;
    }
    else {
        job_id = atoi(arg);
    }
    if (job_id < 1 || job_id > MAX_JOBS) {
        fprintf(stderr, "Invalid job id\n");
        return;
    }
    DEBUG_PRINT("job_id: %d", job_id);
    int pid = jobs[job_id - 1].pid;
    if (pid == 0) {
        fprintf(stderr, "No such job\n");
        return;
    }
    DEBUG_PRINT("pid: %d", pid);
    strcpy(jobs[job_id - 1].status, "Running");
    printf("[%d] %s %d %s\n", pid, jobs[pid].cmd, jobs[pid].pid, jobs[pid].status);
    kill(pid, SIGCONT);
}

/* fg */
void builtin_fg(char *arg)
{
    int job_id;
    if (arg == NULL) {
        job_id = 1;
    }
    else {
        job_id = atoi(arg);
    }
    if (job_id < 1 || job_id > MAX_JOBS) {
        fprintf(stderr, "Invalid job id\n");
        return;
    }
    int pid = jobs[job_id - 1].pid;
    if (pid == 0) {
        fprintf(stderr, "No such job\n");
        return;
    }
    jobs[job_id - 1].pid = 0;
    kill(pid, SIGCONT);
    waitpid(pid, NULL, WUNTRACED);
}