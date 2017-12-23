#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

#define MAXLINE 4096

static void do_grep(FILE *f, const char *match);
static void do_match(const char *match, const char *line);

static int ignore_case = 0;
static int invert_match = 0;

int main(int argc, char *argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, "iv")) != -1) {
    switch (opt) {
      case 'i':
        ignore_case = 1;
        break;
      case 'v':
        invert_match = 1;
        break;
      case '?':
        perror("Unknown option given");
        exit(1);
    }
  }

  if (optind == argc) {
    perror("usage: grep [options] [match] [file, file, ...]");
    exit(1);
  }

  char *match;
  match = argv[optind];

  int i;
  for (i = optind + 1; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");
    if (f == NULL) {
      perror(argv[i]);
      exit(1);
    }
    do_grep(f, match);
    fclose(f);
  }

  exit(0);
}

static void do_grep(FILE *f, const char *match)
{
  char line[MAXLINE];
  while (fgets(line, sizeof line, f) != NULL) {
    do_match(match, line);
  }
}

static void do_match(const char *match, const char *line)
{
  regex_t reg;
  int ok;
  int is_match;
  int cflags;
  cflags = REG_EXTENDED | REG_NEWLINE;
  if (ignore_case) {
    cflags = cflags | REG_ICASE;
  }
  ok = regcomp(&reg, match, cflags);
  if (ok != 0) {
    char buf[1024];
    regerror(ok, &reg, buf, sizeof buf);
    puts(buf);
    exit(1);
  }

  int nmatch = 11;
  regmatch_t pmatch[nmatch];
  is_match = regexec(&reg, line, nmatch, pmatch, 0);
  regfree(&reg);
  int i;
  int j;
  if ((invert_match == 0 && is_match == 0) || (invert_match == 1 && is_match == REG_NOMATCH)) {
    for (i = 0; i < nmatch; i++) {
      if (pmatch[i].rm_so == -1 || pmatch[i].rm_eo == -1) continue;
      for (j = pmatch[i].rm_so; j < pmatch[i].rm_eo; j++) {
        fputc(line[j], stdout);
      }
      fputc('\n', stdout);
    }
  }
}
