#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pid;
  int p1[2], p2[2];
  char byte;
  if (argc != 1) {
    fprintf(2, "usage: pingpong\n");
    exit(1);
  }
  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
    read(p1[0], &byte, 1);
    pid = getpid();
    fprintf(1, "%d: received ping\n", pid);
    write(p2[1], &byte, 1);
  } else {
    write(p1[1], &byte, 1);
    read(p2[0], &byte, 1);
    pid = getpid();
    fprintf(1, "%d: received pong\n", pid);
  }
  close(p1[0]);
  close(p1[1]);
  close(p2[0]);
  close(p2[1]);
  exit(0);
}