#include <stdio.h>
#include <stdlib.h>

static void die(char *c);

#define DEFAULT_DIGIT 16

int main(int argc, char *argv[])
{
  if (argc != 2) die("usage: radix2 12345");

  unsigned long n;
  n = (unsigned long) strtol(argv[1], &argv[1] + sizeof argv[1], 10);

  char *rem;
  rem = (char *) malloc(DEFAULT_DIGIT);
  if (rem == NULL) die("malloc(3)");

  char *rc = rem;
  int q;
  int r;
  int max_size = DEFAULT_DIGIT;
  for (q = n / 2, r = n % 2; q > 0 || r > 0; r = q % 2, q /= 2, ++rc) {
    if ((rc - rem) > max_size) {
      max_size *= 2;
      if (realloc(rem, max_size) == NULL) die("realloc(3)");
    }
    *rc = '0' + r;
  }

  while (rc >= rem) {
    if (fputc(*--rc, stdout) == EOF) die("fputc(3)");
  }

  if (fputc('\n', stdout) == EOF) die("fputc(3)");

  free(rem);
  exit(0);
}

static void die(char *cp)
{
  fprintf(stderr, "Could not %s\n", cp);
  exit(1);
}
