#ifndef _ENV_H_
#define _ENV_H_


// 兼容 Win32 环境，但只有这里兼容了，没什么用
#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#define PATH_LIST_SEPARATOR ";"
#else
#define PATH_SEPARATOR "/"
#define PATH_LIST_SEPARATOR ":" 
#endif // _WIN32

// 限制类
#define LONGEST_CMD_NAME 64
// 环境变量列表
extern char *env_path; // 改名
extern char *env_histfile; // 改名 // 更改初始值
// path 解析相关
extern int path_is_parsed;
extern char **path_list;
extern int path_list_capacity;
extern int path_list_cnt;


// tools
extern int search_external(const char *cmd, char **cmd_path);
extern int parse_path();

#endif
