#include "command_lib.h"
#include "redirect.h"
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


inline void invalid_info(const char *cmd) {
    printf("%s: command not found\n", cmd);
}

// #define redirect(filename, mode, code) do { \
//     switch (code) { \
//         case 0: freopen((filename), (mode), stdin); break; \
//         case 1: freopen((filename), (mode), stdout); break; \
//         case 2: freopen((filename), (mode), stderr); break; \
//     } \
// } while(0)

// #define disredirect(code) do { \
//     if (code != -1) { \
//         switch (code) { \
//             case 0: fclose(stdin); freopen(NULL, "r", stdin); break; \
//             case 1: fclose(stdout); freopen(NULL, "w", stdout); break; \
//             case 2: fclose(stderr); freopen(NULL, "w", stderr); break; \
//         } \
//     } \
// } while(0)
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
    int redirected = 0;

    // preprocess
    for (int i=0; i<cmd_len; i++) {

        // processing quotes and backslashs
        if (!dquoted && !quoted && isblank(cmd[i])) {
            if (i != 0) processed_cmd[pcmd_len++] = '\0';
//             if (redirect_code != -1) {
//                 redirect(redirect_path, "w", redirect_code);
//                 break;
//             }
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


        if (!quoted && !dquoted && (cmd[i]=='>' || cmd[i]=='<')) {
            parse_rdrct(processed_cmd, &pcmd_len, cmd, &i);
            redirected = 1;
            break;
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

//     if (cmd[cmd_len-1]!=' ') {
//         processed_cmd[pcmd_len] = '\0';
//         pcmd_len ++;
//     }
    
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
    int fork_ret;
    if (fork() == 0) {
        execvp(args[0], args);
        exit(0);
    }
    wait(&fork_ret);

    static int counting = 0;
end_success:
    // end
    for (int i=0; args[i]!=NULL; i++) {
        free(args[i]);
        args[i] = NULL;
    }

    if (redirected) {
        reset_fd(); 
    }
    return 1;
}
