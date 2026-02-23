/**
 * builtin echo
 */

#include "../includes/builtins.h"
#include <stdio.h>
#include <string.h>

int builtin_echo(char *const *args) {
    int cnt = 1;
    while (args[cnt] != NULL) {
        printf("%s", args[cnt]);
        if (args[cnt+1] != NULL) putchar(' ');
        cnt ++;
    }
    putchar('\n');
    return 1;
}

