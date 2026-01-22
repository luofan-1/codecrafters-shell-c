#include "../command_lib.h"
#include <stdio.h>

char *builtin_echo(const char *content) {
    printf("%s\n", content);
    return NULL;
}
