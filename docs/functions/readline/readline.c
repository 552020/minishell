#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char* input;

    while (1) {
        input = readline("Enter command: ");
        if (!input) {
            break;  // Exit the loop if end-of-file is reached.
        }

        // Store the line in history.
        add_history(input);

        printf("You entered: %s\n", input);

        free(input);
    }

    return 0;
}

