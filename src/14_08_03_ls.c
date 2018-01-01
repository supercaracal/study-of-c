#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <ctype.h>

static void traverse(const char *dir);
static void write_fi(char *f_path);
static char *trim(char *str);

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "%s\n", "Please specify paths");
    exit(1);
  }

  int i;
  for (i = 1; i < argc; i++) {
    traverse(argv[i]);
  }

  exit(0);
}

#define DIR_BUF 256
static void traverse(const char *path)
{
  DIR *d;
  struct dirent *e;

  d = opendir(path);
  if (d == NULL) {
    perror(path);
    exit(1);
  }

  while ((e = readdir(d)) != NULL) {
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;

    char dir[DIR_BUF];
    strcpy(dir, path);
    if (path[strlen(path) - 1] != '/') strcat(dir, "/");

    switch (e->d_type) {
      case DT_DIR:
        strcat(dir, e->d_name);
        traverse(dir);
        break;
      case DT_REG:
        strcat(dir, e->d_name);
        write_fi(dir);
        break;
    }
  }

  closedir(d);
}

static void write_fi(char *f_path)
{
  struct stat sb;
  if (stat(f_path, &sb) == -1) {
    perror(f_path);
    exit(1);
  }

  struct passwd *pw;
  pw = getpwuid(sb.st_uid);
  if (pw == NULL) {
    perror(f_path);
    exit(1);
  }

  char *m_time = trim(ctime(&sb.st_mtime));

  fprintf(stdout, "%s\t%s\t%s\n", pw->pw_name, m_time, f_path);
}

static char *trim(char *str)
{
  char *first = str;
  while (isspace(*first++)) { }

  char *last = strrchr(str, '\0');
  while (isspace(*--last)) { }

  *++last = '\0';
  return --first;
}
