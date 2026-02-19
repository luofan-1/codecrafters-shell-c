#include "command_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <readline/readline.h>

int get_externals_by_prefix(char **externals, const char *prefix);
char **complete_commands(const char *prefix);
char **complete_paths(const char *prefix);
char **complete(const char *text, int start, int end);

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);
    
    // printf("completion mode: %d", );
    rl_attempted_completion_function = complete;

    char *cmd;
    while (1) {
        cmd = readline("$ ");
        command_parse(cmd);
        free(cmd);
    }

    // char cmd_buff[256];
    // while (1) {
    //     printf("$ ");
    //     fgets(cmd_buff, sizeof(cmd_buff), stdin);
    //     cmd_buff[strlen(cmd_buff) - 1] = '\0';
    //     command_parse(cmd_buff);
    // }

    return 0;
}

char **complete(const char *text, int start, int end) {
    (void)end;
    if (start == 0) {
        return complete_commands(text);
    } else {
        return NULL;
        return complete_paths(text);
    }
}

char **complete_paths(const char *prefix) {

}

// 还没有去重
char **complete_commands(const char *prefix) {
    // builtins
    int list_cnt = 0;
    char **list = (char **)malloc(sizeof(char *)*256); // 越界
    for (int i=0; i<BUILTIN_CNT; i++) {
        if (strstr(cmd_lib[i], prefix) == cmd_lib[i]) {
            list[list_cnt++] = strdup(cmd_lib[i]);
        }
    }

    // externals
    list_cnt += get_externals_by_prefix(list+list_cnt, prefix);

    // end
    if (list_cnt==0) return NULL;
    // list[list_cnt++] = strdup(prefix);
    // qsort(list, list_cnt, sizeof(char *), (int (*)(const void *, const void *))strcmp);
    list[list_cnt] = NULL;
    // printf("list_cnt: %d", list_cnt);
    // for (int i=0; list[i]!=NULL; i++) printf("%d: %s\n", i, list[i]);
    return list;
}


// @return 返回找到的命令数
int get_externals_by_prefix(char **externals, const char *prefix) {
    parse_path();
    DIR *cur_dir = NULL;
    struct dirent *cur_file = NULL;
    int ext_cnt = 0;
    char *full_path = NULL;
    int full_path_capacity = 0;
    for (int i=0; i<path_list_cnt; i++) {
        cur_dir = opendir(path_list[i]);
        cur_file = readdir(cur_dir);
        while (cur_file != NULL) {
            if (strstr(cur_file->d_name, prefix)==cur_file->d_name) {
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
                    externals[ext_cnt++] = strdup(cur_file->d_name);
                }
            }
            cur_file = readdir(cur_dir);
        }
        closedir(cur_dir);
    }
    if (full_path!=NULL) free(full_path);
    return ext_cnt;
}
