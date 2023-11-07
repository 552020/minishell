#include "minishell.h"

// t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
t_debug_level			DEBUG_LEVEL = DEBUG_OFF;
// we could solve it also by returning something by heredoc and checking it before executing
volatile sig_atomic_t	heredoc_interrupted = 0;

int	main(int argc, char **argv, char **envp)
{
	t_env_table	table;
	char		*input;
	size_t		token_count;
	t_token		*token_arr;
	t_lexeme	*lexeme_arr;
	t_ast_node	*ast_root;
	char		**my_envp;
	char		*var_path_value;
	char		*trimmed_input;

	check_input(argc, argv);
	initialize_table(&table, envp);
	my_envp = convert_hash_table_to_array(&table);
	var_path_value = ft_getenv(table.table, "PATH");
	while (1)
	{
		// printf("start of while loop\n");
		handle_signals_main();
		input = read_input();
		trimmed_input = trim_ending_trailing_spaces(input);
		// printf("trimmed_input: %s\n", trimmed_input);
		if (trimmed_input[0] == '\0')
		{
			free(input);
			free(trimmed_input);
		}
		else
		{
			tokenize(&token_count, &token_arr, input);
			if (lexemize(&token_count, &token_arr, &lexeme_arr,
					envp) == SUCCESS)
			{
				parse(&ast_root, lexeme_arr, token_count);
				if (handle_heredocs(ast_root) == SUCCESS)
					execute(ast_root, var_path_value, my_envp, &table);
			}
		}
		//}
	}
	return (0);
}
