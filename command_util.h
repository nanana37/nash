#ifndef _command_util_h
#define _command_util_h

#include "shell.h"

// node for linked list of command and arguments (= command set)
// typedef struct command_and_args {
//     char *token;
//     struct command_and_args *next;
// } command_t;
struct command_and_args {
    char *token;
    command_t *next;
};

// node for linked list of command sets
// typedef struct command_set {
//     command_t *cmd;    // head of command and arguments
//     int size;           // number of commands and arguments
//     char *in;
//     char *out;
//     char *err;
//     struct command_set *next;
// } command_set_t;
struct command_set {
    command_t *cmd;    // head of command and arguments
    int size;           // number of commands and arguments
    char *in;
    char *out;
    char *err;
    struct command_set *next;
};

command_t *init_command();
command_set_t *init_command_set();
void free_command(command_t *command);
void free_command_set(command_set_t *command_set);
void add_command(command_t *cmd, command_t *new_cmd);
void add_command_set(command_set_t *cmd_set, command_set_t *new_cmd_set);

int parse_args(char *args[], command_set_t *cmd_set);

char **get_command(command_set_t *cmd_set);


#endif /* _command_util_h */