#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000
#define LINEBUF 1024
#define NLINE 10

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Specify a filename plz.");
    exit(1);
  }

  FILE *f = fopen(argv[1], "r");

  if (f == NULL) {
    printf("Can not open %s.", argv[1]);
    exit(1);
  }

  char *lines[MAXLINE];
  char buf[LINEBUF];
  int line_cnt = 0;
  while (fgets(buf, sizeof buf, f) != NULL) {
    char *p;
    if ((p = malloc(sizeof buf)) == NULL) {
      perror("Cannot malloc");
      exit(1);
    }
    strcpy(p, buf);
    lines[line_cnt++] = p;
  }

  int i;
  for (i = 0; i < line_cnt; i++) {
    if (i < NLINE) continue;
    if (fputs(lines[i], stdout) == EOF) {
      perror("Cannot fputs");
      exit(1);
    }
  }

  exit(0);
}
