#include "command_lib.h"
#include <unistd.h>
#include <stdio.h>

int builtin_pwd(char *const *args) {
    char buf[256];
    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
    return 1;
}
