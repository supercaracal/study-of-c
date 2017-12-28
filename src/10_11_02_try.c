#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void try_rename_before_close(const char *path);
static void try_unlink_before_close(const char *path);

int main(int argc, char* argv[])
{
  if (argc != 3) {
    perror("Please specify two file paths\n");
    exit(1);
  }

  FILE *f;
  f = fopen(argv[1], "r");
  if (f == NULL) {
    perror(argv[1]);
    exit(1);
  }

  try_rename_before_close(argv[1]);
  try_unlink_before_close(argv[2]);

  fclose(f);
}

static void try_rename_before_close(const char *path)
{
  char c_path[256];

  strcpy(c_path, path);
  strcat(c_path, "_renamed");
  rename(path, c_path);
}

static void try_unlink_before_close(const char *path)
{
  unlink(path);
}
