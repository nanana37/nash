#include "shell.h"

/* Take input */
char *read_line()
{
    char *line = (char*)malloc(MAX_LINE * sizeof(char));
    char *line_tmp = NULL;
    size_t len = 0;
    ssize_t read = 0, tmp;
    int should_read = 1;

    while (should_read) {
        printf("\n");
        switch (should_read) {
            case 1:
                printf("$ ");
                break;
            case 2:
                printf("> ");
                break;
        }
        fflush(stdout);
        read = getline(&line_tmp, &len, stdin);

        if (read == -1) {
            perror("getline");
            exit(1);
        }
        if (line_tmp[read - 1] == ASC_NEWLINE) {
            line_tmp[read - 1] = '\0';
        }
        if (line_tmp[read - 2] == ASC_BACKSLASH) {
            line_tmp[read - 2] = '\0';
            should_read = 2;
        }
        else {
            should_read = 0;
        }

        strcat(line, line_tmp);
    }

    free(line_tmp);

    return line;
}

/* Split command line */
/* pipe & redirection need space
 * like: "cmd1 | cmd2" not: "cmd1|cmd2"
 */
void tokenize_line(char **args, char *line)
{
    char *token;
    int i = 0;

    token = strtok(line, " ");

    while (token != NULL) {
        DEBUG_PRINT("token: %s", token);

        args[i] = token;
        token = strtok(NULL, " ");
        i++;
        if (i > MAX_ARGS) {
            printf("Too many arguments!\n");
            break;
        }
    }
    DEBUG_PRINT("Last token: %s", token);
    args[i] = NULL;
}

