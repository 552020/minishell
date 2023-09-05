#include <stdio.h>
#include <unistd.h>

int main() {
    // Check if stdin is associated with a terminal
    if (isatty(STDIN_FILENO)) {
        printf("stdin is a terminal.\n");
    } else {
        printf("stdin is NOT a terminal.\n");
    }

    // Check if stdout is associated with a terminal
    if (isatty(STDOUT_FILENO)) {
        printf("stdout is a terminal.\n");
    } else {
        printf("stdout is NOT a terminal.\n");
    }

    // Check if stderr is associated with a terminal
    if (isatty(STDERR_FILENO)) {
        printf("stderr is a terminal.\n");
    } else {
        printf("stderr is NOT a terminal.\n");
    }

    return 0;
}

