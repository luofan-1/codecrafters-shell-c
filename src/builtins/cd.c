#include "../includes/builtins.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int builtin_cd(char *const *args) {
    const char *home;

#ifdef _WIN32
    home = getenv("USERPROFILE");
#else
    home = getenv("HOME");
#endif /* ifdef _WIN32 */
    
    // no args
    int ret;
    if (args[1] == NULL) {
        ret = chdir(home);
        return 1;
    }
    
    if (args[2] != NULL) {
        printf("cd: string not in pwd: %s\n", args[1]);
        return 0;
    }

    char to[128]; // attention: exceeding
    int path_len = strlen(args[1]);
    int ppath_len = 0;
    for (int i=0; i<path_len; i++) {
        if (args[1][i]=='~') {
           strcpy(to+ppath_len, home);
           ppath_len += strlen(home);
           to[ppath_len++] = '/';
           continue;
        }
        to[ppath_len++] = args[1][i];
    }
    to[ppath_len] = '\0';

    ret = chdir(to);
    if (ret == -1) {
        printf("cd: %s: No such file or directory\n", to);
        return 0;
    }
    return 1;
}

