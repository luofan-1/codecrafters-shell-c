#include "command_lib.h"
#include "redirect.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

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


inline void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

/**
 * @return 成功则返回1, 失败则返回0
 */

int command_parse(const char *cmd) {
    assert(cmd!=NULL);


    int cmd_len = strlen(cmd);
    char processed_cmd[cmd_len+1];
    int pcmd_len = 0;
    int quoted = 0;
    int dquoted = 0;
    pid_t redirect_pid = -2;
    int pipe_fds[2];
    pid_t pipe_out_pid=-2, pipe_in_pid=-2;
    int is_pipe_child_process = 0;

    // preprocess
    for (int i=0; i<cmd_len; i++) {

        // processing quotes and backslashs
        if (!dquoted && !quoted && isblank(cmd[i])) {
            if (pcmd_len != 0) processed_cmd[pcmd_len++] = '\0';
            while (isblank(cmd[i]) && i<cmd_len) {
                i ++;
            }
            if (i >= cmd_len) break;
        }
        if (!quoted && cmd[i]=='\\') {
            processed_cmd[pcmd_len++] = cmd[++i];
            continue;
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

        // deal with redirections
        if (!quoted && !dquoted && (cmd[i]=='>' || cmd[i]=='<')) {
            redirect_pid = fork();
            if (redirect_pid == 0) {
                parse_redirect(processed_cmd, &pcmd_len, cmd, &i);
                break;
            }
            waitpid(redirect_pid, NULL, 0);
            goto end_success;
        }
        
        // pipes
        if (!quoted && !dquoted && cmd[i]=='|') {
            pipe(pipe_fds);

            pipe_out_pid = fork();
            if (pipe_out_pid == 0) {
                is_pipe_child_process = 1;
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                break;
            }

            pipe_in_pid = fork();
            if (pipe_in_pid == 0) {
                is_pipe_child_process = 1;
                close(pipe_fds[1]);
                dup2(pipe_fds[0], STDIN_FILENO);
                pcmd_len = 0;
                continue;
            }
            
            close(pipe_fds[0]);
            close(pipe_fds[1]);

            waitpid(pipe_out_pid, NULL, 0);
            waitpid(pipe_in_pid, NULL, 0);
            goto end_success;
        }

        // normal characters
        if (dquoted || quoted || (!dquoted && !quoted && !isblank(cmd[i]))) {
            processed_cmd[pcmd_len++] = cmd[i];
            continue;
        }
    }
    if (processed_cmd[pcmd_len-1]!='\0') {
        processed_cmd[pcmd_len] = '\0';
        pcmd_len ++;
    }
    
    // handle with void cmd
    if (strlen(processed_cmd) == 0) {
        return 1;
    }

    // split args
    char *args[64];
    int args_cnt = 0;
    const char *pcmd_ptr = processed_cmd;
    int len_cnt = 0;
    while (len_cnt < pcmd_len) {
        args[args_cnt++] = strdup(pcmd_ptr);
        len_cnt += strlen(pcmd_ptr)+1;
        pcmd_ptr += strlen(pcmd_ptr)+1;
    }
    args[args_cnt] = NULL;
    

// #define DEBUG_PARSE_133
#ifdef DEBUG_PARSE_133
    int i_133 = 0;
    while (args[i_133]!=NULL) {
        printf("%sed\n", args[i_133]);
        i_133 ++;
    }
#endif

    // builtins
    int cmd_idx = get_idx(args[0]);
    if (cmd_idx != -1) {
        (void)cmd_funcs[cmd_idx](args);
        goto end_success;
    }

    // externals
    if (!search_external(args[0], NULL)) {
        invalid_info(args[0]);
        return 0;
    }
    int external_executing_pid = fork();
    if (external_executing_pid == 0) {
        execvp(args[0], args);
        exit(EXIT_SUCCESS);
    }
    waitpid(external_executing_pid, NULL, 0);

    

end_success:
    // end
    for (int i=0; args[i]!=NULL; i++) {
        free(args[i]);
        args[i] = NULL;
    }

    if (redirect_pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    if (is_pipe_child_process) {
        exit(EXIT_SUCCESS);
    }

    // if (pipe_out_pid == 0) {
    //     exit(EXIT_SUCCESS);
    // }

    // if (pipe_in_pid == 0) {
    //     exit(EXIT_SUCCESS);
    // }
    return 1;
}
