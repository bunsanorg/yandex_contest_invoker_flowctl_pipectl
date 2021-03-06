#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <unistd.h>
#include <getopt.h>

static volatile sig_atomic_t nopipe = false;

static bool verbose = false;

static void sigpipe(int sig) {
  (void)sig;
  nopipe = true;
}

int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt) {
      case 'v':
        verbose = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  signal(SIGPIPE, sigpipe);
  char buf[BUFSIZ];
  ssize_t size;
  if (verbose) fprintf(stderr, "Starting read-write loop...\n");
  while (!nopipe && (size = read(0, buf, BUFSIZ))) {
    if (size < 0) {
      perror("read");
      return EXIT_FAILURE;
    }
    ssize_t written = 0;
    if (verbose) fprintf(stderr, "Read %zd bytes.\n", size);
    while (!nopipe && written < size) {
      ssize_t w = write(STDOUT_FILENO, buf + written, size - written);
      if (w < 0) {
        if (!nopipe) {
          perror("write");
          return EXIT_FAILURE;
        } else {
          if (verbose)
            fprintf(stderr, "Discarded %zd bytes.\n", size - written);
          break;
        }
      }
      if (verbose) fprintf(stderr, "Written %zd bytes.\n", w);
      written += w;
    }
  }
  if (size) {
    if (verbose) fprintf(stderr, "Starting discard loop...\n");
    while ((size = read(STDIN_FILENO, buf, sizeof(buf)))) {
      if (size < 0) {
        perror("read");
        return EXIT_FAILURE;
      }
      if (verbose) fprintf(stderr, "Discarded %zd bytes.\n", size);
    }
  }
  if (verbose) fprintf(stderr, "STDIN is empty.\n");
  return EXIT_SUCCESS;
}
