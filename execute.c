#include "execute.h"


void execve_search_path(char *args[])
{
    // char *path = getenv("PATH"); // NOTE: THIS CHANGES ENVIRONMENT VARIABLE
    char *path = (char*)malloc(sizeof(char) * (strlen(getenv("PATH")) + 1));
    memcpy(path, getenv("PATH"), strlen(getenv("PATH")) + 1);

    DEBUG_PRINT("PATH: %s", path);

    char *p = strtok(path, ":");
    while (p != NULL)
    {
        DEBUG_PRINT("Searching %s\n", p);

        char *cmd = (char*)malloc(sizeof(char) * (strlen(p) + strlen(args[0]) + 2));
        memcpy(cmd, p, strlen(p) + 1);
        cmd[strlen(p)] = '/';
        memcpy(cmd + strlen(p) + 1, args[0], strlen(args[0]) + 1);
        DEBUG_PRINT("cmd: %s\n", cmd);

        if (access(cmd, X_OK) == 0) {
            DEBUG_PRINT("found: %s", cmd);
            DEBUG_PRINT("EXECUTING: %s", cmd);
            execute(cmd, args);
            free(cmd);
            return;
        }
        else {
            DEBUG_PRINT("not found: %s", cmd);
            free(cmd);
        }

        p = strtok(NULL, ":");
    }

    fprintf(stderr, "command not found: %s", args[0]);
    return;

}


static void execute(char *cmd, char *args[])
{
    pid_t pid;
    int status;

    DEBUG_PRINT("execute: %s", cmd);

    pid = fork();
    if (pid == 0) {
        /* child */
        //TODO: use execve
        execve(cmd, args, NULL);
        perror("execve");
        exit(1);

        // execvp(cmd, args);
        // perror("execvp");
        // exit(1);
    }
    else if (pid > 0) {
        /* parent */
        DEBUG_PRINT("waiting child pid: %d", pid);
        child_pid = pid;
        waitpid(pid, &status, 0);
    }
    else {
        perror("fork");
        exit(1);
    }
}


//TODO: redirection