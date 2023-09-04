#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Assume stdout is initially the terminal
    const char *tty = ttyname(STDOUT_FILENO);
    if (tty) {
        printf("stdout is initially associated with terminal: %s\n", tty);
    }

    // Redirect stdout to a file
    int fd = open("output.txt", O_WRONLY | O_CREAT, 0666);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    // At this point, stdout is no longer the terminal
    // Let's find out the terminal associated with stdin and reopen it
    tty = ttyname(STDIN_FILENO);
    if (tty) {
        int newfd = open(tty, O_WRONLY);
        if (newfd >= 0) {
            dup2(newfd, STDOUT_FILENO);
            close(newfd);
            printf("stdout is now reset to terminal: %s\n", tty);
        }
    }

    return 0;
}

