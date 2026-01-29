#include "command_lib.h"
#include <unistd.h>
#include <stdio.h>

char *builtin_pwd(const char *args) {
    char buf[256];
    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
    return NULL;
}