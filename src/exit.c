/**
 * builtin exit
 */

#include "command_lib.h"
#include <stdnoreturn.h>
#include <stdlib.h>

int builtin_exit(char *const *args) {
    exit(EXIT_SUCCESS);
    return 0; // 无意义
}
