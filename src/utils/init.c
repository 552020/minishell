#include "minishell.h"

void	initialize_data(t_data *data)
{
	data->env_arr = NULL;
	data->env_table = NULL;
	data->token_arr = NULL;
	data->lexeme_arr = NULL;
	data->ast_root = NULL;

	data->env_table = (t_env_table *)malloc(sizeof(t_env_table));
	if (!data->env_table)
	{
		// Handle memory allocation failure
		printf("Error: Out of memory\n");
		// free memory
		exit(EXIT_FAILURE);
	}
}