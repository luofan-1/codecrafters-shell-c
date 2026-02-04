/**
 * builtin exit 
 */

#include "command_lib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *builtin_echo(const char *content) {
    int len = strlen(content);
    int in_word = 0;
    int quoted = 0;
    for (int i=0; i<len; i++) {
        if (!quoted && content[i]=='\'') {
            quoted = 1;
        }
        if (quoted && content[i]=='\'') {
            quoted = 0;
            // printf(" ");
        }
        if ()
        // 占位
        if (!in_word && isblank(content[i])) {
            continue;
        }
        if (!in_word && !isblank(content[i])) {
            printf("%c", )
        }
        if (in_word) {
            printf("%c", content[i]);
        }
    }
    // printf("%s\n", content);
    return NULL;
}
