/**
 * builtin exit
 */

#include "command_lib.h"
#include <readline/history.h>
#include <stdnoreturn.h>
#include <stdlib.h>

int builtin_exit(char *const *args) {
    for (int i=0; i<path_list_cnt; i++) {
        free(path_list[i]);
    }
    free(path_list);
    write_history(ENV_HISTFILE);
    exit(EXIT_SUCCESS);
    return 0; // 无意义
}
