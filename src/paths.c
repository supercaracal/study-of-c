#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int
main(int argc, char *argv[])
{
  char **p;
  char key[] = "PATH=";

  for (p = environ; *p != NULL; ++p) {
    if (strncmp(*p, key, 5) != 0) {
      continue;
    }

    char *path;
    for (path = *p + 5; *path != '\0'; path++) {
      putc((*path == ':' ? '\n' : *path), stdout);
    }

    putc('\n', stdout);
  }

  exit(0);
}
