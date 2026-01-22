#include "command_lib.h"

const char *ENV_PATH;

#ifdef _WIN32
const char *PATH_LIST_SEPARATOR = ";";
#else
const char *PATH_LIST_SEPARATOR = ":";
#endif