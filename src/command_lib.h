#ifndef _COMMAND_LIB_H_
#define _COMMAND_LIB_H_

#define NDEBUG

#include <stdlib.h>
#include <stdnoreturn.h>


extern const char *ENV_PATH;
extern const char *PATH_SEPARATOR;
extern const char *PATH_LIST_SEPARATOR; 

// 感觉以后用树或者哈希表什么的来替代线性查找会更好一点
#define CMD_LINK(X) \
    X("exit", 0, builtin_exit) \
    X("echo", 1, builtin_echo) \
    X("type", 2, builtin_type) \
    X("pwd",  3, builtin_pwd)


// 命令列表(有点想把这个东西搞到外部去，这样以后添加命令会不会更方便一点？)
#define X(name, idx, func) name,
static char cmd_lib[][64] = {
    CMD_LINK(X)
};
#undef X

#define BUILTIN_CNT (sizeof(cmd_lib)/(sizeof(char)*64))


// commands
// #define TYPE_EXIT "builtin" // 类型宏
extern char *builtin_exit(const char *args); // 命令主函数
// extern void info_exit(); // 帮助函数?

// #define TYPE_ECHO "builtin"
extern char *builtin_echo(const char *content);

// #define TYPE_TYPE "builtin"
extern int search_external(const char *cmd, char **cmd_path);
extern char *builtin_type(const char *args);
#define TYPE(idx) 

// pwd
extern char *builtin_pwd(const char *args);

// 解析我觉得还是要靠一个定的字符指针和偏移量，以后改改
// parse
extern int get_idx(const char *cmd);
extern void invalid_info(const char *cmd);
extern int command_parse(const char *cmd);
extern const char *get_idx_and_move(const char *cmd, int *idx_reciever);

// #define GENERIC_FUNC void*
#define X(name, idx, func) func,
static char *(*cmd_funcs[])(const char *) = {
    CMD_LINK(X)
};
#undef X

#endif