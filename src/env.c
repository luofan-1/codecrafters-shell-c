#include "command_lib.h"

const char *ENV_PATH;

#ifdef _WIN32
const char *PATH_LIST_SEPARATOR = ";";
const char *PATH_SEPARATOR      = "\\";
#else
const char *PATH_LIST_SEPARATOR = ":";
const char *PATH_SEPARATOR      = "/";
#endif