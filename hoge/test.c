#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    char *str = "str";
    printf("%lu\n", strlen(str));

    char **args = argv + 1;
    printf("%s\n",*(argv + 1));

    char *cmd;
    char *path = getenv("PATH");
    char *p = strtok(path, ":");
    while (p != NULL)
    {
        printf("Searching %s\n", p);

        cmd = (char*)malloc(sizeof(char) * (strlen(p) + strlen(argv[1]) + 2));
        memcpy(cmd, p, strlen(p) + 1);
        cmd[strlen(p)] = '/';
        memcpy(cmd + strlen(p) + 1, argv[1], strlen(argv[1]) + 1);
        printf("cmd: %s\n", cmd);
        execve(cmd, argv + 1, envp);

        p = strtok(NULL, ":");
    }
}