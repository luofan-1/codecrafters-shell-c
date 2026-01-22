#include "command_lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    char cmd_buff[256];

#ifndef NDEBUG
    printf("CMDCNT = %d\n", CMDCNT);
#endif

    while (1) {
        printf("$ ");
        fgets(cmd_buff, sizeof(cmd_buff), stdin);
        cmd_buff[strlen(cmd_buff) - 1] = '\0';
        command_parse(cmd_buff);
    }

    return 0;
}
