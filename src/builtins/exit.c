/**
 * builtin exit
 */

#include "../includes/builtins.h"
#include "../includes/env.h"
#include <readline/history.h>
#include <stdnoreturn.h>
#include <stdlib.h>

_Noreturn int builtin_exit(char *const *args) {

    // 释放环境变量 PATH 暂存
    for (int i=0; i<path_list_cnt; i++) {
        free(path_list[i]);
    }
    free(path_list);
    path_list = NULL;
    path_list_cnt = 0;

    // 写入历史记录
    write_history(env_histfile);

    // 退出程序
    exit(EXIT_SUCCESS);
    // return 0; // 无意义
}
