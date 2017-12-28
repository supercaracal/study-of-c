#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static void traverse(const char *dir);

int main(int argc, char *argv[])
{
  if (argc < 2) {
    perror("Please specify paths\n");
    exit(1);
  }

  int i;
  for (i = 1; i < argc; i++) {
    traverse(argv[i]);
  }

  exit(0);
}

static void traverse(const char *path)
{
  DIR *d;
  struct dirent *e;

  d = opendir(path);
  if (d == NULL) {
    perror(path);
    exit(1);
  }

  while ((e = readdir(d)) != NULL) {
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;

    char dir[256];
    strcpy(dir, path);
    if (path[strlen(path) - 1] != '/') strcat(dir, "/");

    switch (e->d_type) {
      case DT_DIR:
        strcat(dir, e->d_name);
        traverse(dir);
        break;
      case DT_REG:
        strcat(dir, e->d_name);
        fputs(dir, stdout);
        fputc('\n', stdout);
        break;
    }
  }

  closedir(d);
}
