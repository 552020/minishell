#include "minishell.h"

// t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
t_debug_level	DEBUG_LEVEL = DEBUG_OFF;

int	main(int argc, char **argv, char **envp)
{
	t_env_table	table;
	char		*input;
	size_t		token_count;
	t_token		*token_arr;
	t_lexeme	*lexeme_arr;
	t_ast_node	*ast_root;
	char		**env_arr;
	char		*var_path_value;
	t_data		data;

	check_input(argc, argv);
	initialize_data(&data, &table);
	initialize_table(&table, envp, &data);
	env_arr = convert_hash_table_to_array(&table, &data);
	var_path_value = ft_getenv(table.table, "PATH");
	while (1)
	{
		input = read_input();
		tokenize(&token_count, &token_arr, input);
		// we are using envp, we should use env_arr
		if (lexemize(&token_count, &token_arr, &lexeme_arr, env_arr) == SUCCESS)
		{
			parse(&ast_root, lexeme_arr, token_count);
			handle_heredocs(ast_root);
			execute(ast_root, var_path_value, env_arr, &table, &data);
		}
		free_ast(ast_root);
	}
	return (0);
}
