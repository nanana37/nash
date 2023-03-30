#include "shell.h"

int main(int argc, char *argv[])
{
    init_shell();

    shell_handler();

    exit_shell();
    return 0;
}
