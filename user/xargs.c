#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[256];
  char arg[128];
  char *args[MAXARG + 1];
  char *p, *s, *d;
  int i, j;
  if (argc <= 1) {
    fprintf(2, "usage: xargs command...\n");
    exit(1);
  }

  for (j = 0, i = 1; i < argc; ++i, ++j) {
    args[j] = (char*)malloc(strlen(argv[i]) + 1);
    strcpy(args[j], argv[i]);
  }
  i = argc - 1;

  p = buf;
  while (read(0, p, 1) == 1) {
    if (*p != '\n') {
      ++p;
      continue;
    }
    *p = ' ';
    for (s = buf, d = arg; s <= p; ++s, ++d) {
      *d = *s;
      if (*s == ' ') {
        *d = '\0';
        args[i] = (char*)malloc(strlen(arg) + 1);
        strcpy(args[i++], arg);
        d = arg;
      }
    }
    args[i] = 0;

    if (fork() == 0) {
      exec(args[0], args);
      exit(0);
    } else {
      wait(0);
    }

    for (j = argc - 1; j < i; ++j) {
      free(args[j]);
    }
    i = argc - 1;
    p = buf;
  }
  exit(0);
}