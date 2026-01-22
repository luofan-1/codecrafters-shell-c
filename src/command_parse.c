#include "command_lib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int get_idx(const char *cmd) {
    for (int i=0; i<CMDCNT; i++)
        if (strstr(cmd, cmd_lib[i]) == cmd) return i;
    return -1;
}

void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

int command_parse(const char *cmd) {
    assert(cmd!=NULL);
    int cmd_idx = get_idx(cmd);
    if (cmd_idx != -1) {
        const char *args = cmd+strlen(cmd_lib[cmd_idx]);
        if (args[0] == ' ') {
            (void)(cmd_funcs[cmd_idx])(args+1);
        } else if (args[0]=='\0') {
            (void)(cmd_funcs[cmd_idx])(args);
        } else {
            goto fail;
        }
        return 1;
    }

fail:
    invalid_info(cmd);
    return 0;
}