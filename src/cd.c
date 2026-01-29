#include "command_lib.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *builtin_cd(const char *args) {
    // chdir 在 windows 下依然是用不了的
    int ret;
    if (strcmp(args, "~") == 0) {
        const char *home;
        #ifdef _WIN32
            home = getenv("USERPROFILE");
        #else
            home = getenv("HOME");
        #endif
        ret = chdir(home);    
    } else {
        ret = chdir(args);
    }
    if (ret == -1) {
        printf("cd: %s: No such file or directory\n", args);
    }

    // printf("cd: %s: No such file or directory\n", args);
    return NULL;
}

/*
cd: <directory>: No such file or directory
*/
// 