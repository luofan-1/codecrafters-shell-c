#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // TODO: Uncomment the code below to pass the first stage
  printf("$ ");
  char buff[256];
  fgets(buff, sizeof(buff), stdin);
  buff[strlen(buff)-1] = '\0';
  printf("%s: command not found\n", buff);
  return 0;
}
