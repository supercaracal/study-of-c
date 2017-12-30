#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int get_opt_line(int argc, char *argv[]);
static int do_cat(int n);

#define DEFAULT_N_LINE 10
#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[])
{
  int n = get_opt_line(argc, argv);
  if (n == -1) {
    fprintf(stderr, "%s", "Could not parse options");
    exit(1);
  }

  int ret = do_cat(n);
  exit(ret);
}

static int get_opt_line(int argc, char *argv[])
{
  int opt;
  int n = DEFAULT_N_LINE;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
      case 'n':
        n = atoi(optarg);
        break;
      case '?':
        n = -1;
        break;
    }
  }
  return n;
}

static int do_cat(int n)
{
  char *ring_buf[n];
  int i;
  int ret = 0;
  for (i = 0; i < n; i++) ring_buf[i] = "";

  char buf[MAX_LINE_LENGTH];
  for (i = 0; fgets(buf, sizeof buf, stdin) != NULL; i++) {
    i = i < n ? i : 0;
    ring_buf[i] = strdup(buf);
  }

  int j;
  for (j = 0; j < n; i++, j++) {
    i = i < n ? i : 0;
    if (fputs(ring_buf[i], stdout) == EOF) {
      fprintf(stderr, "%s", "Could not fputs");
      ret = 1;
      break;
    }
  }

  return ret;
}
