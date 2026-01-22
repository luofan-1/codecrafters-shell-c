#include "command_lib.h"
#include <stdnoreturn.h>
#include <stdlib.h>

_Noreturn void builtin_exit(const char *param) {
    exit(EXIT_SUCCESS);
}