#include "shell.h"

int shell_pid;
int child_pid;
job_t jobs[MAX_JOBS];

/* cd */
void builtin_cd(char *path)
{
    if (path == NULL) {
        path = getenv("HOME");
    }
    if (chdir(path) != 0) {
        perror("chdir");
    }
}

/* pwd */
void builtin_pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
    }
    else {
        printf("%s", cwd);
    }
}

/* echo */
/* default one cant read env */
/* echo only one word */
void builtin_echo(char *str)
{
    if (str == NULL) {
        return;
    }
    if (str[0] == '$') {
        str = getenv(str + 1);
        if (str == NULL) {
            perror("echo");
        } else {
            printf("%s\n", str);
        }
    }
    else {
        printf("%s\n", str);
    }
}

