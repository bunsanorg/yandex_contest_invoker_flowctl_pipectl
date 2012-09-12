#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <unistd.h>
#include <getopt.h>

static volatile sig_atomic_t nopipe = false;

static bool verbose = false;

static void sigpipe(int sig)
{
    (void) sig;
    nopipe = true;
}

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "v")) != -1)
    {
        switch (opt)
        {
        case 'v':
            verbose = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    signal(SIGPIPE, sigpipe);
    char buf[1024];
    ssize_t size;
    while (!nopipe)
    {
        ssize_t written = 0;
        size = read(0, buf, sizeof(buf));
        if (size < 0)
        {
            perror("read");
            return EXIT_FAILURE;
        }
        if (verbose)
            fprintf(stderr, "Read %zd bytes.\n", size);
        while (!nopipe && written < size)
        {
            ssize_t w = write(STDOUT_FILENO, buf + written, size - written);
            if (w < 0)
            {
                if (!nopipe)
                {
                    perror("write");
                    return EXIT_FAILURE;
                }
                else
                {
                    if (verbose)
                        fprintf(stderr, "Discarded %zd bytes.\n", size - written);
                    break;
                }
            }
            if (verbose)
                fprintf(stderr, "Written %zd bytes.\n", w);
            written += w;
        }
    }
    while ((size = read(STDIN_FILENO, buf, sizeof(buf))))
    {
        if (size < 0)
        {
            perror("read");
            return EXIT_FAILURE;
        }
        if (verbose)
            fprintf(stderr, "Discarded %zd bytes.\n", size);
    }
    return EXIT_SUCCESS;
}

