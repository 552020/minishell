#include "minishell.h"

t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
// t_debug_level	DEBUG_LEVEL = DEBUG_OFF;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;

	check_input(argc, argv);
	initialize_data(envp, &data);
	while (1)
	{
		handle_signals_main();
		input = read_input();
		tokenize(&data, input);
		if (lexemize(&data) == SUCCESS)
		{
			parse(&data);
			// handle_heredocs(data.ast_root, &data);
			if (handle_heredocs(data.ast_root, &data) == SUCCESS)
				execute(&data, data.ast_root);
		}
		free_ast(data.ast_root);
	}
	return (0);
}
