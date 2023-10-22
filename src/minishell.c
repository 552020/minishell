#include "minishell.h"

t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
// t_debug_level	DEBUG_LEVEL = DEBUG_OFF;

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
	my_envp = convert_hash_table_to_array(&table);
	my_env_value = ft_getenv(table.table, "PATH");
	while (1)
	{
		input = read_input();
		tokenize(&token_count, &token_arr, input);
		if (lexemize(&token_count, &token_arr, &lexeme_arr, my_envp) == SUCCESS)
		{
			parse(&ast_root, lexeme_arr, token_count);
			if (ast_root->type == N_PIPE)
				handle_pipes(ast_root, my_env_value, my_envp);
			else if (ast_root->type == N_COMMAND)
				handle_without_pipes(ast_root, my_env_value, my_envp);
		}
		free(token_arr);
		free(lexeme_arr);
		// TODO: We need to free the AST
		// free(input);
		// Free memory allocated by readline()
	}
	return (0);
}
