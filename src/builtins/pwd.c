#include "../includes/builtins.h"
#include <unistd.h>
#include <stdio.h>

#define PWD_BUF_SIZE 256

int builtin_pwd(char *const *args) {
    char buf[PWD_BUF_SIZE];
    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
    return 1;
}
