#include "../command_lib.h"
#include <stdio.h>

void builtin_echo(const char *content) {
    if (content[0] == ' ') content ++;
    printf("%s\n", content);
}
