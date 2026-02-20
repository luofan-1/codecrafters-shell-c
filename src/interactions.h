#ifndef _INTERACTIONS_H_
#define _INTERACTIONS_H_

extern void shell_init();
extern char *cmd_gets();
// extern int get_externals_by_prefix(char **externals, const char *prefix);
// extern char **complete_commands(const char *prefix);
// extern char **complete_paths(const char *prefix);
extern char **complete(const char *text, int start, int end);

#endif
