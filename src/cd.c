#include "command_lib.h"
#include <unistd.h>
#include <stdio.h>

char *builtin_cd(const char *args) {
    // chdir 在 windows 下依然是用不了的
    int ret = chdir(args);
    if (ret == -1) {
        printf("cd: %s: No such file or directory\n", args);
    }
    return NULL;
}

/*
cd: <directory>: No such file or directory
*/