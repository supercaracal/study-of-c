#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>

static void do_cat(FILE *f);
static void do_cat_rp(FILE *f);

static struct option longopts[] = {
  { "replace", no_argument, NULL, 'r' },
  { 0, 0, 0, 0 },
};

int main(int argc, char *argv[])
{
  int opt;
  int rpflg = 0;

  while ((opt = getopt_long(argc, argv, "r", longopts, NULL)) != -1) {
    switch (opt) {
      case 'r':
        rpflg = 1;
        break;
      case '?':
        exit(1);
    }
  }

  if (optind == argc) {
    if (rpflg) {
      do_cat_rp(stdin);
    } else {
      do_cat(stdin);
    }
  } else {
    int i;
    for (i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (f == NULL) exit(1);
      if (rpflg) {
        do_cat_rp(f);
      } else {
        do_cat(f);
      }
    }
  }

  exit(0);
}

static void do_cat(FILE *f)
{
  int c;
  while ((c = fgetc(f)) != EOF) {
    fputc(c, stdout);
  }
  fclose(f);
}

static void do_cat_rp(FILE *f)
{
  int c;
  while ((c = fgetc(f)) != EOF) {
    switch (c) {
      case '\t':
        fputc('\\', stdout);
        fputc('t', stdout);
        break;
      case '\n':
        fputc('\\', stdout);
        fputc('n', stdout);
        fputc(c, stdout);
        break;
      default:
        fputc(c, stdout);
        break;
    }
  }
  fclose(f);
}
