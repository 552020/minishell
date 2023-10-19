#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_token			*token_arr;
	t_lexeme		*lexeme_arr;
	t_ast_node		*ast_root;
	size_t			token_count;
	t_debug_level	DEBUG_LEVEL;
	t_env_table		table;
	char			**my_envp;
	char			*my_env_value;

	DEBUG_LEVEL = DEBUG_ALL;
	// char		*key;
	// char		*value;
	// char		**key_value;
	// char		*key_value_str;
	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return (1);
	}
	initialize_table(&table, envp);
	while (1) // Infinite loop to keep the shell running
	{
		input = readline("$> ");
		add_history(input);
		if (!input) // Handle EOF (End Of File, ctrl-D)
		{
			printf("\n");
			break ;
		}
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("readline: %s\n", input);
		/* Tokenization */
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("\n***Tokenization***\n\n");
		token_count = count_words_tokenizer(input);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			printf("Token count: %zu\n\n", token_count);
		token_arr = create_token_array(token_count);
		token_arr = tokenizer(token_arr, input);
		if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
			print_token_arr(token_arr, token_count);
		// i = 0;
		// printf("\n");
		// while (i < token_count + 1)
		// {
		// 	printf("Token %zu: type=%d, str=%s\n", i + 1, token_arr[i].type,
		// 		token_arr[i].str);
		// 	i++;
		// }
		/* Lexing */
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
		// print_ast(ast_root, 7);
		/* execution */
		// Finding PATH environment variable
		// unsigned int idx = hash("PATH");
		// printf("Found PATH environment variable %s\n", table[idx]->value);
		// size_t pipe_count;
		// pipe_count = count_pipes(lexeme_arr, token_count);
		my_envp = convert_hash_table_to_array(&table);
		my_env_value = ft_getenv(table.table, "PATH");
		if (ast_root->type == N_PIPE)
			handle_pipes(ast_root, my_env_value, my_envp);
		else if (ast_root->type == N_COMMAND)
			handle_without_pipes(ast_root, my_env_value, my_envp);
		// printf("");
		/* end of execution */
		free(token_arr);
		free(lexeme_arr);
		// TODO: We need to free the AST
		// free(input); // Free memory allocated by readline()
	}
	return (0);
}
