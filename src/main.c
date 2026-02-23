#include "includes/parser.h"
#include "includes/interactions.h"
#include <readline/readline.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    shell_init();

    char *cmd;
    while (1) {
        cmd = cmd_gets();
        command_parse(cmd);
        free(cmd);
    }

    return 0;
}

