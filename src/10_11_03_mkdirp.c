#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static void do_mkdir(const char *path);
static void try_mkdir(const char *path);
char *do_malloc(size_t s);

int main(int argc, char *argv[])
{
  if (argc < 2) {
    perror("Please specify dir paths\n");
    exit(1);
  }

  int i;
  for (i = 1; i < argc; i++) {
    do_mkdir(argv[i]);
  }

  exit(0);
}

#define MAX_PATH 128

static void do_mkdir(const char *path)
{
  int i;
  int max;
  char c;
  char c_path[MAX_PATH];

  max = strlen(path);
  for (i = 0; i < max; i++) {
    c = path[i];
    c_path[i] = c;
    if (c == '/') {
      char *t_path = do_malloc(i + 2);
      strcpy(t_path, c_path);
      *strrchr(t_path, '/') = '\0';
      try_mkdir(t_path);
      free(t_path);
    }
  }
  if (path[max - 1] != '/') {
    c_path[max] = '\0';
    try_mkdir(c_path);
  }
}

static void try_mkdir(const char *path)
{
  DIR *d;
  int ret;

  d = opendir(path);
  if (d == NULL) {
    ret = mkdir(path, 0777);
    if (ret == -1) {
      perror(path);
      exit(1);
    }
  } else {
    closedir(d);
  }
}

char *do_malloc(size_t s)
{
  void *p;
  p = malloc(s);
  if (p == NULL) {
    perror("Cannot allocate memory\n");
    exit(1);
  }
  return (char *) p;
}
