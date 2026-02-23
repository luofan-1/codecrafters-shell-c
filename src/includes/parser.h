#ifndef _PARSER_H_
#define _PARSER_H_

extern int get_idx(const char *cmd);
extern void invalid_info(const char *cmd);
extern int command_parse(const char *cmd);

#endif
