#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

volatile sig_atomic_t nopipe = false;

void sigpipe(int sig)
{
    (void) sig;
    nopipe = true;
}

int main()
{
    signal(SIGPIPE, sigpipe);
    char buf[1024];
    ssize_t size;
    while (!nopipe)
    {
        ssize_t written = 0;
        size = read(0, buf, sizeof(buf));
        fprintf(stderr, "Read %zd bytes.\n", size);
        while (!nopipe && written < size)
        {
            ssize_t w = write(STDOUT_FILENO, buf + written, size - written);
            fprintf(stderr, "Written %zd bytes.\n", w);
            written += w;
        }
    }
    while (size = read(STDIN_FILENO, buf, sizeof(buf)))
    {
        fprintf(stderr, "Discarded %zd bytes.\n", size);
    }
    return 0;
}

