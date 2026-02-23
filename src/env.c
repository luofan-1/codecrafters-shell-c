#include "includes/builtins.h"
#include "includes/env.h"
#include "includes/parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// 环境变量
char *env_path = NULL;
char *env_histfile = NULL;

// path 解析相关
int path_is_parsed = 0;
char **path_list = NULL;
int path_list_cnt = 0;
int path_list_capacity = 128;

// 工具
static int str_ends_with(const char *str, const char *suffix);

int parse_path() {
    if (env_path == NULL) {
        env_path = getenv("PATH");
    }
    if (path_is_parsed) {
        return path_list_cnt;
    }
    path_list = (char **)malloc(sizeof(char *)*path_list_capacity);
    char *path_copy = strdup(env_path);
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
            // printf("%% accessed %s\n", full_path);
            if (cmd_path != NULL) {
                *cmd_path = full_path;
            } else if (full_path != NULL) {
                free(full_path);
            }
            return 1;
        }
    }
    return 0;

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
