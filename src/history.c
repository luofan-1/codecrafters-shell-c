#include "command_lib.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "readline/history.h"

// abandoned
static int read_history_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);
    
    int history_cnt = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)!=NULL) {
        buf[strlen(buf)-1] = '\0';
        if (buf[0]!='\0') add_history(buf);
        history_cnt ++;
    }
    return history_cnt;
}


int builtin_history(char *const *args) {
    int ret = 1;
    HIST_ENTRY **list = history_list();
    int output_history_num = history_length;
    if (args[1] != NULL) {
        if (strcmp(args[1], "-r")==0) {
            assert(args[2]!=NULL);
            read_history(args[2]);
            return 1;
        } else if (strcmp(args[1], "-w")==0) {
            assert(args[2]!=NULL);
            write_history(args[2]);
            return 1;
        } else if (strcmp(args[1], "-a")==0) {
            assert(args[2]!=NULL);
            append_history(history_length, args[2]);
        }
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
