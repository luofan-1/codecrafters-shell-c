#include "redirect.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define RDRCT_REWRITE 0
#define RDRCT_APPEND  1
#define RDRCT_READ    2


int stdio_fds[] = {
    0, // stdin
    1, // stdout
    2, // stderr
};
// int stdin_fd = 0;
// int stdout_fd = 1;
// int stderr_fd = 2;

int parse_rdrct(char *pcmd, int *len, const char *cmd, int *i) {
    int rdrct_fd = 0;
    int base = 1;
    int has_fd = 0;

    // parse fd
    int last_end = *len;
    while (last_end-1>=0 && isdigit(pcmd[last_end-1])) {
        has_fd = 1;
        rdrct_fd += base*(pcmd[last_end-1]-'0');
        last_end --;
        base *= 10;
    }
    if (!has_fd) rdrct_fd = 1;

    // handle with pcmd_len
    if (pcmd[last_end-1]=='\0') {
        *len = last_end;
    } else {
        pcmd[*len] = '\0'; (*len) ++;
    }

    // check redirect mode
    // 我是不是应该确立一个原则：
    // shell 执行时的命令错误不应该由我来处理
    // 而应该由输入的人承担
    // 这样可以省掉好多边界检测哈哈哈
    int mode;
    if (cmd[*i] == '>') {
        (*i) ++;
        mode = RDRCT_REWRITE;
        if (cmd[*i+1] == '>') {
            mode = RDRCT_APPEND;
            (*i) ++;
        }
    } else {
        mode = RDRCT_READ;
        (*i) ++;
    }
    
    // pass blanks
    while (isblank(cmd[*i])) (*i) ++;

    // parse redirect path
    char rdrct_path[strlen(cmd+(*i))+1];
    int path_len = 0;
    int quoted = 0;
    int dquoted = 0;
    while (cmd[*i]!='\0' && !(!quoted && !dquoted && cmd[*i]==' ')){
        if (!quoted && !dquoted && cmd[*i]=='\"') {
            dquoted = 1;
            goto while_next1;
        }
        // (*i) ++;
        
        if (!quoted && dquoted && cmd[*i]=='\"') {
            dquoted = 0;
            goto while_next1;
        }

        if (!dquoted && !quoted && cmd[*i]=='\'') {
            quoted = 1;
            goto while_next1;
        }

        if (!dquoted && quoted && cmd[*i]=='\'') {
            quoted = 0;
            goto while_next1;
        }

        if (!quoted && cmd[*i]=='\\') {
            (*i) ++;
            rdrct_path[path_len++] = cmd[*i];
            goto while_next1;
        }

        rdrct_path[path_len++] = cmd[*i];

while_next1:
        (*i) ++;
    }
    rdrct_path[path_len] = '\0';

    
    int ret = 0;
    ret = do_redirect(rdrct_path, rdrct_fd, mode);
    // '\0'-12ada>ada 
    // '\0'-12ada12>ada 
    // ada'\0'->ada 
    // ada'\0'-12>ada 
    return ret;
}

int do_redirect(const char *filename, int fd, int mode) {
    int newfd = -1;
    switch (mode) {
        case RDRCT_REWRITE:
            newfd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0664);
            break;
        case RDRCT_APPEND:
            newfd = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0664);
            break;
        case RDRCT_READ:
            break;
    }
    if (newfd == -1) return 0;
    stdio_fds[fd] = dup(fd);
    dup2(newfd, fd);
    close(newfd);
    return 1;
}


// ??? 
int reset_fd() {
    for (int i=0; i<3; i++) {
        if (stdio_fds[i] == i) continue;
        dup2(stdio_fds[i], i);
        close(stdio_fds[i]);
        stdio_fds[i] = i;
    }
    return 1;
}
