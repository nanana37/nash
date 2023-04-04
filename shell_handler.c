#include "shell.h"

void shell_handler()
{
    int should_run = 1; /* flag to determine when to exit program */
    char *cmdline;

    char *args[MAX_ARGS + 1]; /* command line arguments */
    char *token;
    int i;

    command_t *commands = malloc(sizeof(command_t));
    commands->next = NULL;

    while (should_run) {
        printf("\n");
        print_user();
        builtin_pwd();

        // DEBUG_PRINT("Calling read_line...\n");

        mask(SIGINT);
        mask(SIGTSTP);

        cmdline = read_line();
        if (cmdline == NULL) {
            perror("read_line");
            exit(1);
        }

        unmask(SIGINT);
        unmask(SIGTSTP);

        DEBUG_PRINT("cmdline: %s", cmdline);
        // DEBUG_PRINT("cmdline length: %ld", strlen(cmdline));

        /* Split command line */
        tokenize_line(args, cmdline);

        /* Parse args to commands */
        /* First of all, check if calling builtin commands */
        if (args[0] == NULL) {
            continue;
        }
        if (strcmp(args[0], "exit") == 0 || 
            strcmp(args[0], "quit") == 0 ||
            strcmp(args[0], "q") == 0 ||
            strcmp(args[0], "bye") == 0) {
            should_run = 0;
        }
        else if (strcmp(args[0], "cd") == 0) {
            builtin_cd(args[1]);
        }
        else if (strcmp(args[0], "pwd") == 0) {
            builtin_pwd();
            printf("\n");
        }
        //TODO: echo should be able to called with pipe
        else if (strcmp(args[0], "echo") == 0) {
            builtin_echo(args[1]);
        }
        else if (strcmp(args[0], "jobs") == 0) {
            builtin_jobs();
        }
        else if (strcmp(args[0], "bg") == 0) {
            builtin_bg(args[1]);
        }
        else if (strcmp(args[0], "fg") == 0) {
            builtin_fg(args[1]);
        }
        // else if (strcmp(args[0], "history") == 0) {
        //     //TODO
        // }
        else {
            // parse_args(args, commands);
            // execute(args);
            execve_search_path(args);
        }
    }
}
