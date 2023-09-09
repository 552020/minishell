#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// compile with -lreadline: cc readline_signals -lreadline

// In this version when CTRL + D is pressed and the line is empty the program exit but '^D' is printerd by the terminal itself not by readline. 

void handle_sigint(int sig)
{
	(void)sig;
	printf("Received SIGINT (CTRL+C)\n");
	exit(0);
}

void handle_sigquit(int sig)
{
	(void)sig;
	printf("Received SIGQUIT (CTRL+\\)\n");
	exit(0);
}

int main(int argc, char **argv)
{
	char *input;

	if(argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return (1);
	}
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	while(1)
	{
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		printf("Input is: %s\n", input);
		free(input);
	}
	return (0);
}
