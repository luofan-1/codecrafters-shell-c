#ifndef _REDIRECT_H_
#define _REDIRECT_H_


extern int parse_redirect(char *pcmd, int *len, const char *cmd, int *i);

extern int do_redirect(const char *filename, int fd, int mode);


#endif
