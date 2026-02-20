#include "command_lib.h"
#include "interactions.h"
#include <readline/readline.h>


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

