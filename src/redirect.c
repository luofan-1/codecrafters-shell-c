#include "redirect.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define REDIRECT_REWRITE 0
#define REDIRECT_APPEND  1
#define REDIRECT_READ    2

int parse_redirect(char *pcmd, int *len, const char *cmd, int *i) {
    int redirect_fd = 0;
    int base = 1;
    int has_fd = 0;

    // parse fd
    int last_end = *len;
    while (last_end-1>=0 && isdigit(pcmd[last_end-1])) {
        has_fd = 1;
        redirect_fd += base*(pcmd[last_end-1]-'0');
        last_end --;
        base *= 10;
    }

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
        mode = REDIRECT_REWRITE;
        if (!has_fd) redirect_fd = 1;
        if (cmd[*i] == '>') {
            mode = REDIRECT_APPEND;
            (*i) ++;
        }
    } else {
        if (!has_fd) redirect_fd = 0;
        mode = REDIRECT_READ;
        (*i) ++;
    }
    
    // printf("mode:%d\n", mode);
    // pass blanks
    while (isblank(cmd[*i])) (*i) ++;

    // parse redirect path
    char redirect_path[strlen(cmd+(*i))+1];
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
            redirect_path[path_len++] = cmd[*i];
            goto while_next1;
        }

        redirect_path[path_len++] = cmd[*i];

while_next1:
        (*i) ++;
    }
    redirect_path[path_len] = '\0';

    
    int ret = 0;
    ret = do_redirect(redirect_path, redirect_fd, mode);
    return ret;
}

int do_redirect(const char *filename, int fd, int mode) {
    int newfd = -1;
    switch (mode) {
        case REDIRECT_REWRITE:
            newfd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0664);
            break;
        case REDIRECT_APPEND:
            newfd = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0664);
            break;
        case REDIRECT_READ:
            newfd = open(filename, O_RDONLY);
            break;
    }
    if (newfd == -1) return 0;

    dup2(newfd, fd);
    close(newfd);
    return 1;
}


