#include "command_lib.h"
#include <stdio.h>
#include <string.h>

void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

int command_parse(const char *cmd) {
    for (int i = 0; i<CMDCNT; i++) {

#ifndef NDEBUG
    printf("%s-%p:%p\n", cmd, strstr(cmd_lib[i], cmd), cmd_lib[i]);
#endif

        if (strstr(cmd, cmd_lib[i]) == cmd) {
            #define X(name, idx, func) \
                case idx: func(cmd+strlen(cmd_lib[i])); break;
            switch (i) {
                CMD_FUNC(X)
                // default: invalid_info(cmd); return 0; break;
            }
            #undef X
            return 1;
        }
    }
    invalid_info(cmd);
    return 0;
}