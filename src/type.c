/**
 * builtin type
 */

#include "command_lib.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// windows的_access()就是史，别用
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

/**
 * @return 能否找到
 */
int search_external(const char *cmd, char **cmd_path) {
    char *path_copy = strdup(ENV_PATH);
    char *dir = strtok(path_copy, PATH_LIST_SEPARATOR);
    char *full_path = NULL;
    while (dir != NULL) {
        if (dir[strlen(dir)-1] == PATH_SEPARATOR[0]) {
            if (full_path == NULL) {
                full_path = (char *)malloc(strlen(dir)+strlen(cmd)+1);
            } else if (strlen(full_path) < strlen(dir)+strlen(cmd)+1) {
                full_path = realloc(full_path, strlen(dir)+strlen(cmd)+1);
            }
            sprintf(full_path, "%s%s", dir, cmd);
        } else {
            if (full_path == NULL) {
                full_path = (char *)malloc(strlen(dir)+strlen(PATH_SEPARATOR)+strlen(cmd)+1);
            } else if (strlen(full_path) < strlen(dir)+strlen(cmd)+1) {
                full_path = realloc(full_path, strlen(dir)+strlen(PATH_SEPARATOR)+strlen(cmd)+1);
            }
            // full_path = (char *)malloc(strlen(dir)+strlen(PATH_SEPARATOR)+strlen(cmd)+1);
            sprintf(full_path, "%s%s%s", dir, PATH_SEPARATOR, cmd);
        }

        #ifndef NDEBUG
        printf("FULL_PATH = %s\n", full_path);
        #endif

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            // printf("%s is %s\n", cmd, full_path);
            if (cmd_path != NULL) *cmd_path = full_path;
            return 1;
        } 
        // else {
        //     free(full_path);
        // }
        


        dir = strtok(NULL, PATH_LIST_SEPARATOR);
    }
    free(path_copy);
    if (full_path != NULL) {
        free(full_path);
    }
    // 没找到
    // printf("%s: not found\n", args);
    return 0;
}

char *builtin_type(const char *args) {
    // assert(args[0]!='\0');
    // 判断是不是 builtin
    int idx = get_idx(args);
    if (idx != -1) {
        printf("%s is a shell builtin\n", args);
        return NULL;
    }
    
    // 在环境变量 Path 中查找
    // char *path_copy = strdup(ENV_PATH);
    // char *dir = strtok(path_copy, PATH_LIST_SEPARATOR);
    // char full_path[256];
    // while (dir != NULL) {
    //     if (dir[strlen(dir)-1] == PATH_SEPARATOR[0]) {
    //         sprintf(full_path, "%s%s", dir, args);
    //     } else {
    //         sprintf(full_path, "%s%s%s", dir, PATH_SEPARATOR, args);
    //     }

    //     #ifndef NDEBUG
    //     printf("FULL_PATH = %s\n", full_path);
    //     #endif

    //     if (access(full_path, X_OK) == 0) {
    //         free(path_copy);
    //         printf("%s is %s\n", args, full_path);
    //         return NULL;
    //     }
        
    //     dir = strtok(NULL, PATH_LIST_SEPARATOR);
    // }
    // free(path_copy);

    char *path;
    int found = search_external(args, &path);
    if (found) {
        printf("%s is %s\n", args, path);
    } else {
        // 没找到
        printf("%s: not found\n", args);
    }
    
    return NULL;
}