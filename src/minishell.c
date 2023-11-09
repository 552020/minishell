#include "minishell.h"

// t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
t_debug_level	DEBUG_LEVEL = DEBUG_OFF;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;

	check_input(argc, argv);
	initialize_data(envp, &data);
	while (1)
	{
		input = read_input();
		tokenize(&data.token_count, &data.token_arr, input);
		if (lexemize(&data) == SUCCESS)
		{
			parse(&data);
			handle_heredocs(data.ast_root, &data);
			execute(&data);
			// execute(data.ast_root, var_path_value, data.env_arr,
			// data.env_table,
			// &data);
		}
		free_ast(data.ast_root);
	}
	return (0);
}
