#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#include <stdlib.h>
#include <stdnoreturn.h>

// builtins.h
// 感觉以后用树或者哈希表什么的来替代线性查找会更好一点
#define CMD_LINK(X) \
    X("exit",    0, builtin_exit) \
    X("echo",    1, builtin_echo) \
    X("type",    2, builtin_type) \
    X("pwd",     3, builtin_pwd ) \
    X("cd",      4, builtin_cd  ) \
    X("history", 5, builtin_history)

// 命令列表(有点想把这个东西搞到外部去，这样以后添加命令会不会更方便一点？)
#define X(name, idx, func) name,
static const char *cmd_lib[] = {
    CMD_LINK(X)
};
#undef X

#define BUILTIN_CNT (sizeof(cmd_lib)/(sizeof(const char *)))

// commands
// exit
extern _Noreturn int builtin_exit(char *const *args); // 命令主函数

// echo
extern int builtin_echo(char *const *args);

// type
extern int builtin_type(char *const *args);

// pwd
extern int builtin_pwd(char *const *args);

// cd
extern int builtin_cd(char *const *args);

// history
extern int builtin_history(char *const *args);

typedef int (*buitin_cmd)(char *const *args);
#define X(name, idx, func) func,
static buitin_cmd cmd_funcs[] = {
    CMD_LINK(X)
};
#undef X

#endif
