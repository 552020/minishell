#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf("Terminal width: %d\n", w.ws_col);
    printf("Terminal height: %d\n", w.ws_row);

    return 0;
}

