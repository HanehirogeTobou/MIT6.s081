#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int prime, temp;
  int p[2], read_end;
  if (argc != 1) {
    fprintf(2, "usage: primes\n");
    exit(1);
  }
  pipe(p);
  if (fork() != 0) {
    close(p[0]);
    for (prime = 2; prime <= 35; ++prime) {
      write(p[1], &prime, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
  }
  for (;;) {
    close(p[1]);
    if (read(p[0], &prime, 4) == 0) {
      close(p[0]);
      exit(0);
    }
    fprintf(1, "prime %d\n", prime);
    read_end = p[0];
    pipe(p);
    if (fork() != 0) {
      close(p[0]);
      while (read(read_end, &temp, 4) == 4) {
        if (temp % prime == 0) {
          continue;
        }
        write(p[1], &temp, 4);
      }
      close(read_end);
      close(p[1]);
      wait(0);
      exit(0);
    }
  }
}