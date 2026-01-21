#include "command_lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char cmd_buff[256];
  char commands[] = {"exit"};
  char total_cmd = 1;

  while (1) {
    printf("$ ");
    fgets(cmd_buff, sizeof(cmd_buff), stdin);
    cmd_buff[strlen(cmd_buff)-1] = '\0';
    if (strcmp(cmd_buff, "exit") == 0) {
      builtin_exit();
    } else {
      printf("%s: command not found\n", cmd_buff);
    }
  }
  
  return 0;
}
