#include "command_lib.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif

// get command index
int get_idx(const char *cmd) {
    for (int i=0; i<BUILTIN_CNT; i++) {
        if (strcmp(cmd, cmd_lib[i]) == 0) {
            return i;
        }
    }
    
    return -1;
}

// this is shit
// const char *get_idx_and_move(const char *cmd, int *idx_reciever) {
//     *idx_reciever = get_idx(cmd);
//     if (*idx_reciever==-1) return NULL;
//     if (*(cmd+strlen(cmd_lib[*idx_reciever]))==' ') {
//         return cmd+strlen(cmd_lib[*idx_reciever])+1;
//     } else {
//         return cmd+strlen(cmd_lib[*idx_reciever]);
//     }
// }

void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

/**
 * @return 成功则返回1, 失败则返回0
 */

int command_parse(const char *cmd) {
    assert(cmd!=NULL);

#ifdef DEBUG_PARSE
    printf("BUILTIN_CNT: %zu\n", BUILTIN_CNT);
#endif /* ifdef DEBUG_PARSE */

    int cmd_len = strlen(cmd);
    char processed_cmd[cmd_len+1];
    int pcmd_len = 0;
    int quoted = 0;
    int dquoted = 0;
    // preprocess
    for (int i=0; i<cmd_len; i++) {
        if (!dquoted && !quoted && isblank(cmd[i])) {
            if (i != 0) processed_cmd[pcmd_len++] = '\0';
            while (isblank(cmd[i]) && i<cmd_len) {
                i ++;
            }
            if (i >= cmd_len) break;
        }
        
        if (!quoted && !dquoted && cmd[i]=='\"') {
            dquoted = 1;
            continue;
        }

        if (!quoted && dquoted && cmd[i]=='\"') {
            dquoted = 0;
            continue;
        }

        if (!dquoted && !quoted && cmd[i]=='\'') {
            quoted = 1;
            continue;
        }
        if (!dquoted && quoted && cmd[i]=='\'') {
            quoted = 0;
            continue;
        }

        if (dquoted || quoted || (!dquoted && !quoted && !isblank(cmd[i]))) {
            processed_cmd[pcmd_len++] = cmd[i];
            continue;
        }
    }
    if (cmd[cmd_len-1]!=' ') {
        processed_cmd[pcmd_len] = '\0';
        pcmd_len ++;
    }
    
    // handle with void cmd
    if (strlen(processed_cmd) == 0) {
        return 1;
    }

#ifdef DEBUG_PARSE
    printf("%s@\n", processed_cmd);
#endif /* ifdef DEBUG_PARSE */
    // split args
    char *args[64];
    int args_cnt = 0;
    const char *pcmd_ptr = processed_cmd;
    int len_cnt = 0;
    while (len_cnt < pcmd_len) {
        // args[args_cnt++] = (char *)malloc(strlen(pcmd_ptr)+1);
        // strcat(args[args_cnt], pcmd_ptr);
        // printf("fault\n");
        args[args_cnt++] = strdup(pcmd_ptr);
        len_cnt += strlen(pcmd_ptr)+1;
        pcmd_ptr += strlen(pcmd_ptr)+1;
    }
    args[args_cnt] = NULL;
 
#ifdef DEBUG_PARSE
    printf("-------------------- DEBUG_PARSE_83 -------------------\n");
    int cnt_83 = 0;
    while (args[cnt_83]!=NULL) {
        printf("%s@\n", args[cnt_83++]);
    }
    printf("-------------------- DEBUG_PARSE_83 -------------------\n");
#endif /* ifdef DEBUG_PARSE_83 */

    // builtins
    int cmd_idx = get_idx(args[0]);
    if (cmd_idx != -1) {
        (void)cmd_funcs[cmd_idx](args);
        return 1;
    }

    // externals
    if (!search_external(args[0], NULL)) {
        invalid_info(args[0]);
        return 0;
    }

#ifndef _WIN32 
    int fork_ret;
    if (fork() == 0) {
        execvp(args[0], args);
        return 0;
    }
    wait(&fork_ret);
#endif /* ifndef _WIN32  */
    

    // end
    for (int i=0; args[i]!=NULL; i++) {
        free(args[i]);
        args[i] = NULL;
    }
    return 1;
}
