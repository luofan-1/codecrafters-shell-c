#include "command_lib.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Flush after every printf
    ENV_PATH = getenv("PATH");
    setbuf(stdout, NULL);
    char cmd_buff[256];

#ifndef NDEBUG
    printf("PATH = %s\n", ENV_PATH);
    printf("BUILTIN_CNT = %zu\n", BUILTIN_CNT);
#endif
    while (1) {
        printf("$ ");
        fgets(cmd_buff, sizeof(cmd_buff), stdin);
        cmd_buff[strlen(cmd_buff) - 1] = '\0';
        command_parse(cmd_buff);
    }

    return 0;
}
