#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// cc minishell.c -lreadline

// You can choose the size of this buffer according to your needs.
#define MAX_PATH_SIZE 1024 

void print_working_directory() {
    char buffer[MAX_PATH_SIZE];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("%s\n", buffer);
    } else {
        perror("Error getting current working directory");
    }
}


int main(void)
{
    char *input;
    while (1) // Infinite loop to keep the shell running
    {
        input = readline("$> "); // Display prompt and read input from user
        add_history(input); // Add input to history
        if (!input) // Handle EOF (End Of File, ctrl-D)
        {
            printf("\n");
            break;
        }
        printf("%s\n", input); // Echo input
        if (strcmp(input, "pwd") == 0) {
            print_working_directory();
        } else {
            printf("Unknown command\n");
            }
        free(input); // Free memory allocated by readline()
    }
    return 0;
}

