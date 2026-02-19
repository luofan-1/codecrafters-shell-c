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

static int str_ends_with(const char *str, const char *suffix);

int parse_path() {
    if (ENV_PATH == NULL) {
        ENV_PATH = getenv("PATH");
    }
    if (path_is_parsed) {
        return path_list_cnt;
    }
    path_list = (char **)malloc(sizeof(char *)*path_list_capacity);
    char *path_copy = strdup(ENV_PATH);
    char *cur_dir = strtok(path_copy, PATH_LIST_SEPARATOR);
    while (cur_dir!=NULL) {
        if (path_list_cnt > path_list_capacity) {
            path_list_capacity *= 2;
            path_list = realloc(path_list, path_list_capacity*sizeof(char *));
        }

        path_list[path_list_cnt++] = strdup(cur_dir);

        // 加入 '/'
        if (!str_ends_with(cur_dir, PATH_SEPARATOR)) {
            int newlen = strlen(cur_dir)+strlen(PATH_SEPARATOR)+1;
            path_list[path_list_cnt-1] = realloc(path_list[path_list_cnt-1], newlen);
            strcat(path_list[path_list_cnt-1], PATH_SEPARATOR);
        }
        
        cur_dir = strtok(NULL, PATH_LIST_SEPARATOR);
        
    }
    free(path_copy); path_copy=NULL; cur_dir=NULL;
    path_is_parsed = 1;
    return path_list_cnt;
}

/**
 * @return 能否找到
 */
int search_external(const char *cmd, char **cmd_path) {
    parse_path();
    char *full_path = NULL;
    for (int i=0; i<path_list_cnt; i++) {
        int path_len = strlen(path_list[i])+strlen(cmd);
        
        // realloc it if the memory isn't enough
        if (full_path == NULL) {
            full_path = (char *)malloc(path_len+1);
        } else if (strlen(full_path) < path_len+1) {
            full_path = (char *)realloc(full_path, path_len+1);
        }

        sprintf(full_path, "%s%s", path_list[i], cmd);

        if (access(full_path, X_OK)==0) {
            printf("%% accessed %s\n", full_path);
            if (cmd_path != NULL) {
                *cmd_path = full_path;
            } else if (full_path != NULL) {
                free(full_path);
            }
            return 1;
        }
    }
    return 0;

    // char *path_copy = strdup(ENV_PATH);
    // char *dir = strtok(path_copy, PATH_LIST_SEPARATOR);
    // char *full_path = NULL;
    // while (dir != NULL) {
    //     
    //     int path_len = strlen(dir)+strlen(cmd);
    //     // handle with the situation that the path ends with SEPARATORS
    //     if (dir[strlen(dir)-1] != PATH_SEPARATOR[0]) {
    //         path_len += strlen(PATH_SEPARATOR);
    //     }
    //     
    //     // realloc it if the memory isn't enough
    //     if (full_path == NULL) {
    //         full_path = (char *)malloc(path_len+1);
    //     } else if (strlen(full_path) < path_len+1) {
    //         full_path = realloc(full_path, path_len+1);
    //     }

    //     // to add SEPARATORS or not
    //     if (dir[strlen(dir)-1] == PATH_SEPARATOR[0]) {
    //         sprintf(full_path, "%s%s", dir, cmd);
    //     } else { // not ends with SEPARATORS
    //         // full_path = (char *)malloc(strlen(dir)+strlen(PATH_SEPARATOR)+strlen(cmd)+1);
    //         sprintf(full_path, "%s%s%s", dir, PATH_SEPARATOR, cmd);
    //     }

    //     #ifndef NDEBUG
    //     printf("FULL_PATH = %s\n", full_path);
    //     #endif
    //     
    //     // check
    //     if (access(full_path, X_OK) == 0) {
    //         free(path_copy);
    //         // printf("%s is %s\n", cmd, full_path);
    //         if (cmd_path != NULL) *cmd_path = full_path;
    //         return 1;
    //     } 
    //     
    //     // moving to next path
    //     dir = strtok(NULL, PATH_LIST_SEPARATOR);
  //   }

    // free(path_copy);
    // if (full_path != NULL) {
    //     free(full_path);
  //   }
  //   // 没找到
  //   // printf("%s: not found\n", args);
  //   return 0;
}


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

static int str_ends_with(const char *str, const char *suffix) {
    int len_suffix = strlen(suffix);
    int len_str = strlen(str);
    if (len_suffix>len_str) {
        return 0;
    }
    for (int i=1; i<=len_suffix; i++) {
        if (str[len_str-i]!=suffix[len_suffix-i]) {
            return 0;
        }
    }
    return 1;
}
