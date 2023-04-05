#include "execute.h"
#include "command_util.h"

extern int num_of_commands;

void execve_search_path(char *args[])
{
    // char *path = getenv("PATH"); // NOTE: THIS CHANGES ENVIRONMENT VARIABLE
    char *path = (char*)malloc(sizeof(char) * (strlen(getenv("PATH")) + 1));
    memcpy(path, getenv("PATH"), strlen(getenv("PATH")) + 1);

    // DEBUG_PRINT("PATH: %s", path);

    char *p = strtok(path, ":");
    while (p != NULL)
    {
        // DEBUG_PRINT("Searching %s\n", p);

        char *cmd = (char*)malloc(sizeof(char) * (strlen(p) + strlen(args[0]) + 2));
        memcpy(cmd, p, strlen(p) + 1);
        cmd[strlen(p)] = '/';
        memcpy(cmd + strlen(p) + 1, args[0], strlen(args[0]) + 1);
        // DEBUG_PRINT("cmd: %s\n", cmd);

        if (access(cmd, X_OK) == 0) {
            // DEBUG_PRINT("found: %s", cmd);
            execute(cmd, args);
            free(cmd);
            return;
        }
        else {
            // DEBUG_PRINT("not found: %s", cmd);
            free(cmd);
        }

        p = strtok(NULL, ":");
    }

    fprintf(stderr, "command not found: %s\n", args[0]);
    return;

}


static void execute(char *cmd, char *args[])
{
    pid_t pid;
    int status;

    DEBUG_PRINT("EXECUTING: %s", cmd);

    pid = fork();
    if (pid == 0) {
        /* child */
        // DEBUG_PRINT("child excuting: %s", cmd);
        execve(cmd, args, NULL);
        perror("execve");
        exit(1);

        // execvp(cmd, args);
        // perror("execvp");
        // exit(1);
    }
    else if (pid > 0) {
        /* parent */
        // DEBUG_PRINT("waiting child pid: %d", pid);
        child_pid = pid;
        waitpid(pid, &status, 0);
    }
    else {
        perror("fork");
        exit(1);
    }
}


void execute_all_commands(command_set_t *cmd_set)
{
    int i;
    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * num_of_commands);
    if (pids == NULL) {
        perror("malloc");
        exit(1);
    }
    //TODO: is status necessary?

    // DEBUG_PRINT("num_of_commands: %d", num_of_commands);

    int **pipefds = (int **)malloc(sizeof(int *) * num_of_commands);
    if (pipefds == NULL) {
        perror("malloc");
        exit(1);
    }
    for (i = 0; i < num_of_commands; i++) {
        pipefds[i] = (int *)malloc(sizeof(int) * 2);
        if (pipefds[i] == NULL) {
            perror("malloc");
            exit(1);
        }
        if (pipe(pipefds[i]) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    command_set_t *cur_cmd_set = cmd_set;
    for (i = 0; i < num_of_commands; i++) {
        if (cur_cmd_set == NULL) {
            fprintf(stderr, "execute_all_commands: cur_cmd_set is NULL\n");
            exit(1);
        }
        pids[i] = fork();
        if (pids[i] == 0) {
            /* child */
            break;
        }
        else if (pids[i] > 0) {
            /* parent */
            cur_cmd_set = cur_cmd_set->next;
        }
        else {
            perror("fork");
            exit(1);
        }
    }

    if (i == num_of_commands) {
        DEBUG_PRINT("parent");
        /* parent */
        for (i = 0; i < num_of_commands; i++) {
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }
        for (i = 0; i < num_of_commands; i++) {
            waitpid(pids[i], NULL, 0);
            // wait(NULL);
            DEBUG_PRINT("child %d finished", pids[i]);
        }
        // wait(NULL);
        // while(waitpid(-1, NULL, WNOHANG) > 0){}
    }
    else if (i >= 0 && i < num_of_commands) {
        DEBUG_PRINT("child");

        /* Backup for standard I/O/E */
        int bu_in = dup(STDIN_FILENO);
        int bu_out = dup(STDOUT_FILENO);
        int bu_err = dup(STDERR_FILENO);

        /* cmd1 | cmd2 | cmd3 */
        /* OUT -> 1 pipe 0 -> IN */
        /* bind input */
        if (cur_cmd_set->in != NULL) {
            int fd = open(cur_cmd_set->in, O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(fd);
        }
        if (i != 0) {
            /* not first child */
            if (dup2(pipefds[i - 1][0], STDIN_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(pipefds[i - 1][0]);
        }

        /* bind output */
        if (cur_cmd_set->out != NULL) {
            int fd = open(cur_cmd_set->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(fd);
        }
        if (i != num_of_commands - 1) {
            /* not last child */
            if (dup2(pipefds[i][1], STDOUT_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(pipefds[i][1]);
        }
        
        /* bind err */
        if (cur_cmd_set->err != NULL) {
            int fd = open(cur_cmd_set->err, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(1);
            }
            if (dup2(fd, STDERR_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(fd);
        }

        execve_search_path(get_command(cur_cmd_set));

        DEBUG_PRINT("Initialize Standard I/O/E");

        /* Init Standard I/O/E */
        dup2(bu_in, STDIN_FILENO);
        if (dup2(bu_in, STDIN_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        if (dup2(bu_out, STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        if (dup2(bu_err, STDERR_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        close(bu_in);
        close(bu_out);
        close(bu_err);

        exit(0);
    }
    
    return;
}