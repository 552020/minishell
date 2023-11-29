#include "minishell.h"

// t_debug_level	DEBUG_LEVEL = DEBUG_ALL;
t_debug_level	DEBUG_LEVEL = DEBUG_OFF;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	int		exit_status;

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
			{
				execute(&data, data.ast_root);
				wait_ast(&data, data.ast_root);
				exit_status = data.ast_root->exit_status;
				printf("exit_status: %d\n", exit_status);
				data.last_exit_status = exit_status;
			}
		}
		if (data.ast_root)
			free_ast(data.ast_root);
		data.ast_type = UNDEFINED;
	}
	return (exit_status);
}
