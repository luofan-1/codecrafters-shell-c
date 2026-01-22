#include "../command_lib.h"
#include <assert.h>
// #include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// #include <stdlib.h>

char *builtin_type(const char *args) {
    // assert(args[0]!='\0');
    // 判断是不是 builtin
    int idx = get_idx(args);
    if (idx != -1) {
        printf("%s is a shell builtin\n", args);
        return NULL;
    }
    
    // 在环境变量 Path 中查找
    char *path_copy = strdup(ENV_PATH);
    char *dir = strtok(path_copy, PATH_LIST_SEPARATOR);
    char full_path[256];
    while (dir != NULL) {
        #ifdef _WIN32
            sprintf(full_path, "%s\\%s", dir, args);
        #else
            sprintf(full_path, "%s/%s", dir, args);
        #endif

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            printf("%s is %s\n", args, full_path);
            return NULL;
        }
        
        dir = strtok(NULL, PATH_LIST_SEPARATOR);
    }
    free(path_copy);

    // 没找到
    printf("%s: not found\n", args);
    return NULL;
}