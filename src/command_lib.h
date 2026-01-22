#ifndef _COMMAND_LIB_H_
#define _COMMAND_LIB_H_

#define NDEBUG

#include <stdnoreturn.h>

// 感觉以后用树或者哈希表什么的来替代线性查找会更好一点
#define CMD_FUNC(X) \
    X(exit, 0, builtin_exit) \
    X(echo, 1, builtin_echo) \
    X(type, 2, builtin_type)


// 命令列表(有点想把这个东西搞到外部取,这样以后添加命令会不会更方便一点?)
#define X(name, idx, func) #name,
static char cmd_lib[][64] = {
    CMD_FUNC(X)
};
#undef X

#define CMDCNT (sizeof(cmd_lib)/(sizeof(char)*64))
#define STRTO

// commands
#define TYPE_EXIT "builtin" // 类型宏
extern _Noreturn void builtin_exit(const char *args); // 命令主函数
// extern void info_exit(); // 帮助函数?

#define TYPE_ECHO "builtin"
extern void builtin_echo(const char *content);

#define TYPE_TYPE "builtin"
extern void builtin_type(const char *args);
#define TYPE(idx) 

// parse
extern void invalid_info(const char *cmd);
extern int command_parse(const char *cmd);

#endif