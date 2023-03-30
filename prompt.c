#include "shell.h"


void print_user(void)
{
    char *user = getenv("USER");
    if (user == NULL) {
        perror("getenv");
    }
    else {
        printf("%s:", user);
    }
}

void init_shell(void)
{
    printf("Welcome to nash!\n");
    printf("Type 'exit' or 'quit' to exit.\n");

    shell_pid = getpid();
    init_jobs();

    /* Ignore SIGINT */
    change_sigint_handler();

    /* Ignore SIGTSTP */
    change_sigtstp_handler();
}

void exit_shell(void)
{
    //TODO: free memory

    // kill all child processes (including zombies & bg processes)
    //TODO: This prints "Killed" and reaches "Bye!"
    kill(0, SIGKILL);

    printf("Bye!\n");
}
