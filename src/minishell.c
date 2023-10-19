#include "minishell.h"

t_debug_level	DEBUG_LEVEL = DEBUG_ALL;

int	main(int argc, char **argv, char **envp)
{
	t_env_table	table;
	char		*input;
	size_t		token_count;
	t_token		*token_arr;
	t_lexeme	*lexeme_arr;
	t_ast_node	*ast_root;
	char		**my_envp;
	char		*my_env_value;

	check_input(argc, argv);
	initialize_table(&table, envp);
	while (1)
	{
		input = read_input();
		tokenize_main(&token_count, &token_arr, input, DEBUG_LEVEL);
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
