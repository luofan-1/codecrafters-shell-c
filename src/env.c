#include "command_lib.h"
#include <stdlib.h>

char *ENV_PATH = NULL;
char *ENV_HISTFILE = NULL;
int path_is_parsed = 0;
char **path_list = NULL;
int path_list_cnt = 0;
int path_list_capacity = 128;
