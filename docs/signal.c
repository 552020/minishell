#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Signal %d received!\n", signum);
}

int main() {
    signal(SIGINT, handler);  // Set handler function for SIGINT (CTRL+C)

    while (1) {
        printf("Running...\n");
        sleep(1);
    }

    return 0;
} 
