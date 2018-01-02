#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

static int open_connection(char *host, char *service);
static int do_echo(FILE *f);

int main(int argc, char *argv[])
{
  char *host;
  int sock;
  FILE *f;

  host = argc > 1 ? argv[1] : "localhost";
  sock = open_connection(host, "echo");
  if (sock == EOF) {
    exit(1);
  }

  f = fdopen(sock, "w+");
  if (f == NULL) {
    perror("fdopen(3)");
    return 1;
  }

  int ret = do_echo(f);

  if (fclose(f) == EOF) {
    perror("fclose(3)");
    return 1;
  }

  exit(ret);
}

static int open_connection(char *host, char *service)
{
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *ai;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int err;
  if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
    return EOF;
  }

  int sock;
  for (ai = res; ai; ai = ai->ai_next) {
    sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sock < 0) {
      perror("socket(2)");
      continue;
    }
    if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
      perror("connect(2)");
      close(sock);
      continue;
    }
    freeaddrinfo(res);
    return sock;
  }

  fprintf(stderr, "Could not connect echo server\n");
  freeaddrinfo(res);
  return EOF;
}

#define MAX_LINE 1024
static int do_echo(FILE *f)
{
  char buf[MAX_LINE];

  if (fgets(buf, sizeof buf, stdin) == NULL) {
    fprintf(stderr, "Could not %s from client\n", "fgets(3)");
    return 1;
  }

  if (fputs(buf, f) == EOF) {
    fprintf(stderr, "Could not %s from server\n", "fputs(3)");
    return 1;
  }

  if (fgets(buf, sizeof buf, f) == NULL) {
    fprintf(stderr, "Could not %s from server\n", "fgets(3)");
    return 1;
  }

  if (fputs(buf, stdout) == EOF) {
    fprintf(stderr, "Could not %s from client\n", "fputs(3)");
    return 1;
  }

  return 0;
}
