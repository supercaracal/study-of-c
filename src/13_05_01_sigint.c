#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

static void handler(int sig);
static void trap(int sig, sighandler_t handler);

int main(int argc, char *argv[])
{
  trap(11, handler);

  int ret = pause();
  if (ret  == -1) {
    perror("Returned from pause(2)");
    exit(0);
  }

  exit(0);
}

static void handler(int sig)
{
  fprintf(stdout, "Signal %d trapped.\n", sig);
}

static void trap(int sig, sighandler_t handler)
{
  struct sigaction act;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;

  if (sigaction(11, &act, NULL) < 0) {
    perror("Could not sigaction(2)");
    exit(1);
  }
}
