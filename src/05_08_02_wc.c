#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static void do_wc_l(const char *path);
static void die(const char *s);

int main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    do_wc_l("");
  } else {
    for (i = 1; i < argc; i++) {
      do_wc_l(argv[i]);
    }
  }

  exit(0);
}

#define BUFFER_SIZE 2048

static void do_wc_l(const char *path)
{
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;
  int m;
  int cnt;
  int i;

  if (strlen(path) > 0) {
    fd = open(path, O_RDONLY);
  } else {
    fd = STDIN_FILENO;
  }

  cnt = 0;
  m = 0;

  for (;;) {
    n = read(fd, buf, sizeof buf);

    if (n < 0) die(path);
    if (n == 0) break;

    for (i = m; i < n; i++) {
      if (buf[i] == '\n') cnt++;
      m = i;
    }
  }

  printf("%d", cnt);

  if (close(fd) < 0) die(path);
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
