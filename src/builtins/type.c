#include "../command_lib.h"
#include <assert.h>
#include <stdio.h>
// #include <stdlib.h>

char *builtin_type(const char *args) {
    // assert(args[0]!='\0');
    int idx = get_idx(args);
    if (idx != -1) {
        printf("%s is a shell builtin\n", args);
    } else {
        printf("%s: not found\n", args);
    }
    return NULL;
}