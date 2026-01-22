#include "command_lib.h"
#include <stdnoreturn.h>
#include <stdlib.h>

char *builtin_exit(const char *args) {
    exit(EXIT_SUCCESS);
    return NULL; // 无意义
}