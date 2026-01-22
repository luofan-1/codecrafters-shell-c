#ifndef _COMMAND_LIB_H_
#define _COMMAND_LIB_H_

#define NDEBUG

#include <stdnoreturn.h>

// 感觉以后用树或者哈希表什么的来替代线性查找会更好一点
#define CMD_FUNC(X) \
    X("exit", 0, builtin_exit) \
    X("echo", 1, buitin_echo)


// 命令列表
#define X(cstr, cidx, cfunc) cstr,
static char cmd_lib[][64] = {
    CMD_FUNC(X)
};
#undef X

#define CMDCNT (sizeof(cmd_lib)/(sizeof(char)*64))

extern _Noreturn void builtin_exit(const char *param);
extern void buitin_echo(const char *content);

extern void invalid_info(const char *cmd);
extern int command_parse(const char *cmd);

#endif