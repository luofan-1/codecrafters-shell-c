#include "../command_lib.h"
#include <stdnoreturn.h>
#include <stdlib.h>

_Noreturn void builtin_exit(const char *args) {
    exit(EXIT_SUCCESS);
}