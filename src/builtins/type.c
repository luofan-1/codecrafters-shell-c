/**
 * builtin type
 */

#include "../includes/builtins.h"
#include "../includes/env.h"
#include "../includes/parser.h"
#include <assert.h>
#include <stdio.h>

// windows的适配待实现
#ifdef _WIN32
    #include <io.h>
    #define access _access
    #define F_OK 0
    #define R_OK 4
    #define W_OK 2
    #define X_OK 1
#else
    #include <unistd.h>
#endif

int builtin_type(char *const *args) {
    if (args[1] == NULL) return 0;
    int ret = 1;
    for (int i=1; args[i]!=NULL; i++) {
    
        // check if it is builtins
        int idx = get_idx(args[i]);
        if (idx != -1) {
            printf("%s is a shell builtin\n", cmd_lib[idx]);
            continue;
        }

        // search for externals in PATH
        char *path;
        int is_external = search_external(args[i], &path);
        if (is_external) {
            printf("%s is %s\n", args[i], path);
            continue;
        } else {
            ret = 0;
            printf("%s: not found\n", args[i]);
            continue;
        }
    }
    return ret;
}
