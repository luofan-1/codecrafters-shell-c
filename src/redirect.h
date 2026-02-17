#ifndef _REDIRECT_H_
#define _REDIRECT_H_

extern int stdio_fds[];

extern int parse_rdrct(char *pcmd, int *len, const char *cmd, int *i);

extern int do_redirect(const char *filename, int fd, int mode);

extern int reset_fd();

#endif
