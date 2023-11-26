#include "minishell.h"

char	*read_input(void)
{
	char *input;
	input = readline("$> ");
	add_history(input);
	if (!input) // Handle EOF (End Of File, ctrl-D)
	{
		printf("exit\n");
		// if (data)
		//	free_data(data);

		exit(0);
		// free_exit(data, "Exit\n");
		// ft_exit(0, data->ast_root, data->env_arr, data->env_table);
	}
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("readline: %s\n", input);
	return (input);
}