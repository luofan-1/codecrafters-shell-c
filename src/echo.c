/**
 * builtin exit 
 */

#include "command_lib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *builtin_echo(const char *content) {
    int len = strlen(content);
    int in_word = 1;
    int quoted = 0;
    for (int i=0; i<len; i++) {
        
        if (!quoted && isblank(content[i])) {
            putchar(' ');
            while (isblank(content[i]) && i<len) {
                i ++;
            }
        }

        if (!quoted && content[i]=='\'') {
            quoted = 1;
            continue;
        }
        if (quoted && content[i]=='\'') {
            quoted = 0;
            continue;
        }

        if (!quoted || (quoted&&!isblank(content[i]))) {
            putchar(content[i]);
            continue;
        }

    }
    putchar('\n');
    return NULL;
}
