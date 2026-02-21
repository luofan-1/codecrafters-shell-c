#include "command_lib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "readline/history.h"

int builtin_history(char *const *args) {
    int ret = 1;
    HIST_ENTRY **list = history_list();
    if (list == NULL) return 0;
    int output_history_num = history_length;
    if (args[1] != NULL) {
        output_history_num = 0;
        int len_limit = strlen(args[1]);
        for (int i=0; i<len_limit; i++) {
            output_history_num = output_history_num*10+args[1][i]-'0';
        }
    }
    int history_cnt = history_length-output_history_num;
    if (history_cnt < 0) {
        history_cnt = 0;
        ret = 0;
    }
    while (list[history_cnt] != NULL) {
        printf("    %d  %s\n", history_cnt+1, list[history_cnt]->line);
        history_cnt ++;
    }
    return ret;
}
