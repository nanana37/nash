#include "command_util.h"

/* Parsing */

int num_of_commands = 0;

command_t *init_command()
{
    command_t *cmd = malloc(sizeof(command_t));
    cmd->token = NULL;
    cmd->next = NULL;
    return cmd;
}

command_set_t *init_command_set()
{
    command_set_t *cmd_set = malloc(sizeof(command_set_t));
    cmd_set->cmd= init_command();
    cmd_set->size = 0;
    cmd_set->in = NULL;
    cmd_set->out = NULL;
    cmd_set->err = NULL;
    cmd_set->next = NULL;
    return cmd_set;
}


void free_command(command_t *cmd)
{
    if (cmd->next != NULL) {
        free_command(cmd->next);
    }
    else {
        return;
    }
    free(cmd);
}

void free_command_set(command_set_t *cmd_set)
{
    if (cmd_set->next != NULL) {
        free_command_set(cmd_set->next);
    }
    else {
        num_of_commands = 0;
        return;
    }
    // free_command(cmd_set->cmd);
    free(cmd_set);
}

void add_command(command_t *cmd, command_t *new_cmd)
{
    command_t *cur = cmd;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_cmd;
}

void add_command_set(command_set_t *cmd_set, command_set_t *new_cmd_set)
{
    command_set_t *cur = cmd_set;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_cmd_set;
}

int parse_args(char *args[], command_set_t *cmd_set)
{
    command_t *cmd = init_command();
    command_set_t *cur_set = cmd_set;
    cur_set->cmd = cmd;
    num_of_commands++;

    int i;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            add_command_set(cmd_set, init_command_set());
            num_of_commands++;
            cur_set = cur_set->next;
            cmd = cur_set->cmd;
        }
        else if (strcmp(args[i], "<") == 0) {
            cur_set->in = args[++i];
        }
        else if (strcmp(args[i], ">") == 0) {
            cur_set->out = args[++i];
        }
        else if (strcmp(args[i], "2>") == 0) {
            cur_set->err = args[++i];
        }
        else {
            add_command(cmd, init_command());
            cmd->token = args[i];
            cur_set->size++;
            cmd = cmd->next;
        }
    }
    
    if (strcmp(args[i - 1], "|") == 0) {
        num_of_commands = -1;
    }

    return num_of_commands;
}

char **get_command(command_set_t *cmd_set)
{
    char **args = (char **)malloc(sizeof(char *) * (cmd_set->size + 1));
    command_t *cmd = cmd_set->cmd;
    int i = 0;
    while (cmd != NULL) {
        args[i++] = cmd->token;
        cmd = cmd->next;
    }
    return args;    // Last token of args is always NULL
}