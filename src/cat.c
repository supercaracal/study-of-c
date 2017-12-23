#include <stdio.h>
#include <stdlib.h>

static void do_cat(char path[]);

int main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    printf("Please specify file paths\n");
    exit(1);
  }

  for (i = 1; i < argc; i++) {
    do_cat(argv[i]);
  }

  exit(0);
}

static void do_cat(char path[])
{
  FILE *f;
  int c;

  f = fopen(path, "r");

  while ((c = fgetc(f)) != EOF) {
    if (putchar(c) < 0) {
      perror(path);
      fclose(f);
      exit(1);
    }
  }

  fclose(f);
}
