#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f);

int main(int argc, char *argv[])
{
  if (argc == 1) {
    do_cat(stdin);
  } else {
    FILE *f;
    for (int i = 1; i < argc; i++) {
      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_cat(f);
      fclose(f);
    }
  }

  exit(0);
}

#define MAXLENGTH 2048

static void do_cat(FILE *f)
{
  unsigned char buf[MAXLENGTH];

  while (1) {
    size_t n_read = fread(buf, 1, sizeof buf, f);
    if (ferror(f)) exit(1);
    size_t n_written = fwrite(buf, 1, n_read, stdout);
    if (n_written < n_read) exit(1);
    if (n_read < sizeof buf) break;
  }
}
