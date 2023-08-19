#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// cc minishell.c -lreadline

// You can choose the size of this buffer according to your needs.
#define MAX_PATH_SIZE 1024

void	print_working_directory(void)
{
	char	buffer[MAX_PATH_SIZE];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		printf("%s\n", buffer);
	}
	else
	{
		perror("Error getting current working directory");
	}
}

int	main(void)
{
	char	*input;
	t_token	*token_arr;
	size_t	token_count;
	size_t	i;

	while (1) // Infinite loop to keep the shell running
	{
		input = readline("$> "); // Display prompt and read input from user
		add_history(input);      // Add input to history
		if (!input)              // Handle EOF (End Of File, ctrl-D)
		{
			printf("\n");
			break ;
		}
		printf("%s\n", input); // Echo input
		token_count = count_words_tokenizer(input);
		token_arr = tokenizer(input);
		i = 0;
		while (i < token_count)
		{
			printf("Token %zu: type=%d, str=%s\n", i, token_arr[i].type,
				token_arr[i].str);
			i++;
		}
		free(token_arr);
		// Call your tokenizer function
		if (strcmp(input, "pwd") == 0)
		{
			print_working_directory();
		}
		else
		{
			printf("Unknown command\n");
		}
		free(input); // Free memory allocated by readline()
	}
	return (0);
}
