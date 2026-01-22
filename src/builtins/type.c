#include "../command_lib.h"
#include <stdlib.h>

void builtin_type(const char *args) {
    if (args == NULL) return;
    if (args[0] == ' ') {
        args ++;
    }
    for (int i=0; i<CMDCNT; i++) {
        if (strstr(args, cmd_lib[i]) == args) {
            printf("%s is a shell %s", cmd_lib[i], );
        }
    }
    printf("%s: not found\n", args);
}