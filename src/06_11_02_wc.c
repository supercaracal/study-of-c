#include <stdio.h>
#include <stdlib.h>

static void do_wc(FILE *f);

int main(int argc, char *argv[])
{
  if (argc == 1) {
    do_wc(stdin);
  } else {
    int i;
    for (i = 1; i < argc; i++) {
      FILE *f;
      if ((f = fopen(argv[i], "r")) == NULL) {
        perror(argv[i]);
        exit(1);
      }
      do_wc(f);
      if (fclose(f) != 0) {
        perror(argv[i]);
        exit(1);
      }
    }
  }
}

#define MAX_LENGTH 2048

static void do_wc(FILE *f)
{
  char buf[MAX_LENGTH];
  int count;

  while (fgets(buf, sizeof buf, f) != NULL) {
    ++count;
  }

  printf("%d\n", count);
}
