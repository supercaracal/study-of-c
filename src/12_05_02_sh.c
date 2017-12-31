#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

static char *read_line(void);
static char **split_cmd_line(char *c_line);
static char **malloc_argv(void);
static char **malloc_x(void);
static char *malloc_y(void);
static void do_cmd(char *argv[]);
static void wait_child_process(pid_t pid);

int main(int argc, char* argv[])
{
  char *c_line;
  pid_t pid;

  if (argc > 1) {
    do_cmd(&argv[1]);
    exit(0);
  }

  while (1) {
    printf("%s", "\n> ");
    c_line = read_line();
    if (c_line == NULL) continue;

    pid = fork();
    if (pid < 0) {
      perror(c_line);
      exit(1);
    }

    if (pid == 0) {
      char **cmd = split_cmd_line(c_line);
      if (cmd == NULL) continue;
      do_cmd(cmd);
    } else {
      wait_child_process(pid);
    }
  }
}

#define MAX_CMD_LEN 1024

static char *read_line(void)
{
  char *c_line;
  char buf[MAX_CMD_LEN];

  c_line = fgets(buf, sizeof buf, stdin);
  if (c_line == NULL) {
    fprintf(stderr, "Could not execute `%s`", "fgets(3)");
    return NULL;
  }

  return c_line;
}

static char **split_cmd_line(char *c_line)
{
  char **argv = malloc_argv();
  if (argv == NULL) return NULL;

  int i, j;
  for (i = 0; *c_line; i++, c_line++) {
    for (j = 0; *c_line != ' '; j++, c_line++) {
      argv[i][j] = *c_line;
    }
    argv[i][j] = '\0';
  }
  argv[i] = NULL;

  return argv;
}

static char **malloc_argv(void)
{
  char **argv = malloc_x();
  if (argv == NULL) return NULL;

  int i;
  for (i = 0; i < sizeof argv; i++) {
    argv[i] = malloc_y();
  }
  return argv;
}

#define MAX_ARGC 20
static char **malloc_x(void)
{
  char **x = (char **) calloc(1, MAX_ARGC);
  if (x == NULL) fprintf(stderr, "Could not %s", "malloc(3)");
  return x;
}

#define MAX_ARG_LEN 128
static char *malloc_y(void)
{
  char *y = (char *) calloc(1, MAX_ARG_LEN);
  if (y == NULL) fprintf(stderr, "Could not %s", "malloc(3)");
  return y;
}

static void do_cmd(char *argv[])
{
  execv(argv[0], argv);
  perror("Could not execv(3)");
  exit(99);
}

static void wait_child_process(pid_t pid)
{
  int status;
  waitpid(pid, &status, 0);
  if (WIFEXITED(status)) {
    fprintf(stderr, "exit, status=%d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
    fprintf(stderr, "signal, sig=%d, %s\n", sig, strsignal(sig));
  } else {
    fprintf(stderr, "%s", "abnormal exit\n");
  }
}
