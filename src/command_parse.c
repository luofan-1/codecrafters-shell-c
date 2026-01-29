#include "command_lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif

// get command index
int get_idx(const char *cmd) {
    char nextch;
    for (int i=0; i<BUILTIN_CNT; i++) {
        nextch = *(cmd+strlen(cmd_lib[i]));
        if (strstr(cmd, cmd_lib[i])==cmd && (nextch==' '||nextch=='\0')) return i;
    }
    return -1;
}

// ...
const char *get_idx_and_move(const char *cmd, int *idx_reciever) {
    *idx_reciever = get_idx(cmd);
    if (*idx_reciever==-1) return NULL;
    if (*(cmd+strlen(cmd_lib[*idx_reciever]))==' ') {
        return cmd+strlen(cmd_lib[*idx_reciever])+1;
    } else {
        return cmd+strlen(cmd_lib[*idx_reciever]);
    }
}

void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

int command_parse(const char *cmd) {
    assert(cmd!=NULL);
    // int cmd_idx = get_idx(cmd);
    // // builtins
    // // 这里要改成get idx and move吗
    // if (cmd_idx != -1) {
    //     const char *args = cmd+strlen(cmd_lib[cmd_idx]);
    //     if (args[0] == ' ') {
    //         (void)(cmd_funcs[cmd_idx])(args+1);
    //     } else if (args[0]=='\0') {
    //         (void)(cmd_funcs[cmd_idx])(args);
    //     } else {
    //         goto fail;
    //     }
    //     return 1;
    // }

    // -------------------新增---------------------
    int cmd_idx;
    const char *args = get_idx_and_move(cmd, &cmd_idx);
    if (cmd_idx != -1) {
        (void)(cmd_funcs[cmd_idx])(args);
        return 1;
    }
    // -------------------新增---------------------

//     char *path;
//     if (search_external(cmd, &path)) {
//         system(cmd);
//         return 1;
//     }
    
// fail:
//     invalid_info(cmd);
    

    char *tokens[64];
    char *cmd_copy = strdup(cmd);
    tokens[0] = strtok(cmd_copy, " ");
    int tok_cnt = 1;
    while (tokens[tok_cnt-1]!=NULL && tok_cnt<=64) {
        tokens[tok_cnt++] = strtok(NULL, " ");
    }

    
    if (!search_external(tokens[0], NULL)) {
        // goto fail;
        invalid_info(tokens[0]);
        return 0;
    }

    #ifdef __WIN32
        // 待改进
        system(cmd);
    #else
        int fork_ret;
        if (fork() == 0) {
            execvp(tokens[0], tokens);
            // printf("");
            // goto fail;
            exit(1);
        }
        wait(&fork_ret);
    #endif
    
// fail:
    // invalid_info(tokens[0]);

    return 1;
}