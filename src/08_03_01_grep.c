#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

#define MAXLINE 4096

static void do_grep(FILE *f, const char *match);
static int do_match(const char *match, const char *line);

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
  int is_match;
  while (fgets(line, sizeof line, f) != NULL) {
    is_match = do_match(match, line);
    if ((invert_match == 0 && is_match == 0) || (invert_match == 1 && is_match == REG_NOMATCH)) {
      fputs(line, stdout);
    }
  }
}

static int do_match(const char *match, const char *line)
{
  regex_t reg;
  int ok;
  int is_match;
  int cflags;
  cflags = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
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
  is_match = regexec(&reg, line, 0, NULL, 0);
  regfree(&reg);
  return is_match;
}
