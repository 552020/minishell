#include "minishell.h"

void	initialize_data(t_data *data, t_env_table *env_table)
{
	data->env_arr = NULL;
	data->env_table = env_table;
	data->ast_root = NULL;
}