#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* filename(char *path) {
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; --p);
  return ++p;
}

void find(char *path, char *name) {
  char *buf, *p;
  int fd;
  struct dirent de;
  struct stat st;
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot fstat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type)
  {
  case T_FILE:
    if (strcmp(filename(path), name) == 0) {
      fprintf(1, "%s\n", path);
    }
    break;

  case T_DIR:
    buf = (char*)malloc(strlen(path) + DIRSIZ + 2);
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0) {
        continue;
      }
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name);
    }
    free(buf);
    break;

  default:
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "usage: find directory filename\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}