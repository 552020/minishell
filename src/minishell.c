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
	t_ast_node	*ast_root;
	size_t		token_count;
	size_t		i;
	t_env_var	*table[TABLE_SIZE];

	// char		*key;
	// char		*value;
	// char		**key_value;
	// char		*key_value_str;
	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return (1);
	}
	initialize_table(table, envp);
	while (1) // Infinite loop to keep the shell running
	{
		input = readline("$> "); // Display prompt and read input from user
		add_history(input);      // Add input to history
		if (!input)              // Handle EOF (End Of File, ctrl-D)
		{
			printf("\n");
			break ;
		}
		// Handle 'env' command - just for testing
		if (ft_strncmp(input, "env", ft_strlen(input)) == 0)
		{
			printf("env command\n");
			env(table);
			continue ;
		}
		// Handle 'export' command - just for testing
		// if (ft_strncmp(input, "export ", 7) == 0)
		// {
		// 	key_value_str = ft_strdup(input + 7);
		// 	key_value = ft_split(key_value_str, '=');
		// 	if (key_value && key_value[0] && key_value[1])
		// 	{
		// 		key = key_value[0];
		// 		value = key_value[1];
		// 		export(table, key, value);
		// 	}
		// 	free(key_value_str);
		// 	free(key_value[0]);
		// 	free(key_value[1]);
		// 	free(key_value);
		// 	continue ;
		// }
		// Handle  'unset' command - just for testing
		// if (ft_strncmp(input, "unset ", 6) == 0)
		// {
		// 	key = ft_strdup(input + 6);
		// 	unset(table, key);
		// 	free(key);
		// 	continue ;
		// }
		printf("readline: %s\n", input);
		/* Tokenization */
		printf("\n***Tokenization***\n\n");
		token_count = count_words_tokenizer(input);
		printf("Token count: %zu\n\n", token_count);
		token_arr = tokenizer(input);
		i = 0;
		while (i < token_count + 1)
		{
			printf("Token %zu: type=%d, str=%s\n", i + 1, token_arr[i].type,
				token_arr[i].str);
			i++;
		}
		/* Lexing */
		printf("***Lexing***\n\n");
		lexeme_arr = lexer(token_arr, envp, token_count);
		i = 0;
		printf("***Parsing***\n\n");
		ast_root = build_ast(lexeme_arr, 0, token_count - 1);
		print_ast(ast_root, 7);
		/* execution */
		// Finding PATH environment variable
		// unsigned int idx = hash("PATH");
		// printf("Found PATH environment variable %s\n", table[idx]->value);
		// size_t pipe_count;
		// pipe_count = count_pipes(lexeme_arr, token_count);
		if (ast_root->type == N_PIPE)
			handle_pipes(ast_root, table[hash("PATH")]->value, envp);
		else if (ast_root->type == N_COMMAND)
			handle_without_pipes(ast_root, table[hash("PATH")]->value, envp);
		// printf("");
		/* end of execution */
		free(token_arr);
		free(lexeme_arr);
		// if (strcmp(input, "pwd") == 0)
		// {
		// 	print_working_directory();
		// }
		// else
		// {
		// 	printf("Unknown command\n");
		// }
		// free(input); // Free memory allocated by readline()
	}
	return (0);
}
