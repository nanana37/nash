#include "execute.h"


void execve_search_path(char *args[])
{
    char *cmd;
    char *path = getenv("PATH");
    DEBUG_PRINT("PATH: %s", path);
    char *p = strtok(path, ":");
    while (p != NULL)
    {
        DEBUG_PRINT("Searching %s\n", p);

        cmd = (char*)malloc(sizeof(char) * (strlen(p) + strlen(args[0]) + 2));
        memcpy(cmd, p, strlen(p) + 1);
        cmd[strlen(p)] = '/';
        memcpy(cmd + strlen(p) + 1, args[0], strlen(args[0]) + 1);
        DEBUG_PRINT("cmd: %s\n", cmd);

        if (access(cmd, X_OK) == 0) {
            DEBUG_PRINT("found: %s", cmd);
            DEBUG_PRINT("EXECUTING: %s", cmd);
            return;
        }
        else {
            DEBUG_PRINT("not found: %s", cmd);
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



//TODO: pipe　動作未確認（pipe分割できてないので
//TODO: command_tのようい（pipeで分割）
//TODO: これだとredできない
void exec_with_pipe(command_t *commands)
{
    command_t *cmd = commands;

    pid_t pid[MAX_CHLD];
    int i, parent;
    int pipefds[2];

    if (pipe(pipefds) < 0) {
        perror("pipe");
        exit(1);
    }

    for (i = 0; i < MAX_CHLD; i++) {
        pid[i] = fork();

        if (pid[i] == 0) {
            break; // child
        }
        else if (pid[i] < 0) {
            perror("fork");
            exit(1);
        }
    }

    for (i = 0; i < MAX_CHLD; i++) {
        if (cmd == NULL) {
            parent = i;
        }
        
        pid[i] = fork();
        if (pid[i] == 0) {
            break; // child
        }
        else if (pid[i] < 0) {
            perror("fork");
            exit(1);
        }

        cmd = cmd->next;
    }

    if (i == MAX_CHLD) {
        fprintf(stderr, "too many children\n");
        exit(1);
    }

    // switch (i) {
    //     case 0:
    //         close(pipefds[0]);
    //         if (dup2(pipefds[1], STDOUT_FILENO) < 0) {
    //             perror("dup2");
    //             exit(1);
    //         }
    //         close(pipefds[1]);
    //         break;
    //     case parent - 1:
    //         close(pipefds[1]);
    //         if (dup2(pipefds[0], STDIN_FILENO) < 0) {
    //             perror("dup2");
    //             exit(1);
    //         }
    //         close(pipefds[0]);
    //         break;
    //     case parent:
    //         close(pipefds[0]);
    //         close(pipefds[1]);
    //         for (int j = 0; j < parent; j++) {
    //             wait(NULL):
    //         }
    //         break;
    //     case default:
    //         if (dup2(pipefds[0], STDIN_FILENO) < 0) {
    //             perror("dup2");
    //             exit(1);
    //         }
    //         if (dup2(pipefds[1], STDOUT_FILENO) < 0) {
    //             perror("dup2");
    //             exit(1);
    //         }
    //         close(pipefds[0]);
    //         close(pipefds[1]);
    //         break;
    // }

}


//TODO: redirection