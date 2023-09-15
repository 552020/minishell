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
	char			*input;
	t_token			*token_arr;
	t_lexeme		*lexeme_arr;
	t_ast_node		*ast_root;
	size_t			token_count;
	size_t			i;
	t_env_var		*table[TABLE_SIZE];
	t_debug_level	DEBUG_LEVEL;

	DEBUG_LEVEL = DEBUG_OFF;
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
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("readline: %s\n", input);
		/* Tokenization */
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("\n***Tokenization***\n\n");
		token_count = count_words_tokenizer(input);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("Token count: %zu\n\n", token_count);
		token_arr = tokenizer(input);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			print_token_arr(token_arr, token_count);
		i = 0;
		while (i < token_count + 1)
		{
			printf("Token %zu: type=%d, str=%s\n", i + 1, token_arr[i].type,
				token_arr[i].str);
			i++;
		}
		/* collecting the heredoc */
		printf("\n*Heredoc*\n\n");
		collect_heredoc_content(token_arr, token_count);
		/* Lexing */
		printf("***Lexing***\n\n");
		lexeme_arr = lexer(token_arr, envp, token_count);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
			printf("***Parsing***\n\n");
		ast_root = build_ast(lexeme_arr, 0, token_count - 1);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
		{
			printf("\n***Printing AST***\n\n");
			print_ast(ast_root, 7);
			printf("\n***Printing AST NEW***\n\n");
			print_ast_new(ast_root);
			printf("\n*** AST nodes content ***\n\n");
			debug_ast(ast_root);
		}
		free(token_arr);
		free(lexeme_arr);
		// TODO: We need to free the AST
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
