#include "includes/builtins.h"
#include "includes/env.h"
#include "includes/interactions.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

static char **completion_list = NULL;
static int completion_list_capacity = 128;
static int completion_list_cnt = 0;

static char *generate_command(const char *text, int state);
static int list_commands(const char *prefix);
static int get_externals_by_prefix(const char *prefix);

void shell_init() {
    setbuf(stdout, NULL);
    // readline 相关
    rl_attempted_completion_function = complete;
    completion_list = (char **)malloc(sizeof(char *)*completion_list_capacity);
    env_histfile = getenv("HISTFILE");
    read_history(env_histfile);
}

char *cmd_gets() {
    char *line_read = readline("$ ");
    if (line_read && *line_read) {
        add_history(line_read);
    }
    return line_read;
}

static char *generate_command(const char *text, int state) {
    static int index;

    if (state == 0) {
        index = 0;
        list_commands(text);
    }

    if (index < completion_list_cnt) {
        return strdup(completion_list[index++]);
    }

    return NULL;
}

char **complete(const char *text, int start, int end) {
    (void)end;
    if (start != 0) return NULL;
    return rl_completion_matches(text, generate_command);
}


// 还没有去重
static int list_commands(const char *prefix) {
    completion_list_cnt = 0;
    // builtins
    for (int i=0; i<BUILTIN_CNT; i++) {
        if (strncmp(cmd_lib[i], prefix, strlen(prefix))==0) {
            if (completion_list_cnt >= completion_list_capacity) {
                completion_list_capacity *= 2;
                completion_list = realloc(completion_list, sizeof(char *)*completion_list_capacity);
            }
            completion_list[completion_list_cnt++] = strdup(cmd_lib[i]);
        }
    }

    // externals
    get_externals_by_prefix(prefix);

    // end
    if (completion_list_cnt != 0)
        qsort(completion_list, completion_list_cnt, sizeof(char *), (int (*)(const void *, const void *))strcmp);
    // completion_list[completion_list_cnt] = NULL;
    // for (int i=0; completion_list[i]!=NULL; i++) printf("%d: %s\n", i, completion_list[i]);

    // printf("list_commands: function completed\nlist_cnt: %d", list_cnt);
    return completion_list_cnt;
}


static int get_externals_by_prefix(const char *prefix) {
    parse_path();
    DIR *cur_dir = NULL;
    struct dirent *cur_file = NULL;
    char *full_path = NULL;
    int full_path_capacity = 0;
    for (int i=0; i<path_list_cnt; i++) {
        cur_dir = opendir(path_list[i]);
        cur_file = readdir(cur_dir);
        while (cur_file != NULL) {
            if (strncmp(cur_file->d_name, prefix, strlen(prefix))==0) {
                // 扩容
                int required_capacity = strlen(path_list[i])+strlen(cur_file->d_name)+1;
                if (full_path == NULL) {
                    full_path = (char *)malloc(required_capacity);
                    full_path_capacity = required_capacity;
                } else if (required_capacity > full_path_capacity) {
                    full_path = (char *)realloc(full_path, required_capacity);
                    full_path_capacity = required_capacity;
                }
                // 复制
                strcpy(full_path, path_list[i]);
                strcat(full_path, cur_file->d_name);
                // 访问
                if (access(full_path, X_OK)==0) {
                    // 扩容
                    if (completion_list_cnt >= completion_list_capacity) {
                        completion_list_capacity *= 2;
                        completion_list = realloc(completion_list, sizeof(char *)*completion_list_capacity);
                    }
                    completion_list[completion_list_cnt++] = strdup(cur_file->d_name);
                }
            }
            cur_file = readdir(cur_dir);
        }
        closedir(cur_dir);
    }
    if (full_path!=NULL) free(full_path);
    return 1;
}
