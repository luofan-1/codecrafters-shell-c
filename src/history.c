#include "command_lib.h"
#include <stdio.h>
#include <time.h>
#include "readline/history.h"

int builtin_history(char *const *args) {
    HIST_ENTRY **list = history_list();
    if (list == NULL) return 0;
    
    int history_cnt = 0;
    while (list[history_cnt] != NULL) {
        printf("    %d  %s\n", history_cnt+1, list[history_cnt]->line);
        history_cnt ++;
    }
    return 1;
}
