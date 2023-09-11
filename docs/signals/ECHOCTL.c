#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void suppress_echoctl() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void restore_echoctl() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    char *input;
    
    suppress_echoctl();  // Suppress the echoing of ^D

    while (1) {
        input = readline("Enter input (CTRL+D to exit): ");

        if (!input) {  // CTRL+D was pressed
            printf("\nExit\n");
            break;
        }
        
        printf("You entered: %s\n", input);
        free(input);
    }

    restore_echoctl();  // Restore the echoing behavior

    return 0;
}

