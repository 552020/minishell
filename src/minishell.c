#include "minishell.h"

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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*token_arr;
	t_lexeme	*lexeme_arr;
	size_t		token_count;
	size_t		i;

	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return (1);
	}
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
		/* Tokenization */
		token_count = count_words_tokenizer(input);
		printf("Token count: %zu\n", token_count);
		token_arr = tokenizer(input);
		i = 0;
		while (i < token_count + 1)
		{
			printf("Token %zu: type=%d, str=%s\n", i, token_arr[i].type,
				token_arr[i].str);
			i++;
		}
		/* collecting the heredoc */
		collect_heredoc_content(token_arr, token_count);
		/* Lexing */
		lexeme_arr = lexer(token_arr, envp, token_count);
		i = 0;
		while (i < token_count + 1)
		{
			printf("Lexeme %zu: type=%d, value=%s, original=%s\n", i,
				lexeme_arr[i].type, lexeme_arr[i].str, lexeme_arr[i].original);
			i++;
		}
		free(token_arr);
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
