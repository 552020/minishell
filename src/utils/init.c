#include "minishell.h"

void	initialize_envp_arr(t_data *data)
{
	data->env_arr = hash_table_to_arr(data);
}

void	initialize_data(char **envp, t_data *data)
{
	data->env_arr = NULL;
	data->env_table = NULL;
	data->token_arr = NULL;
	data->lexeme_arr = NULL;
	data->ast_root = NULL;

	data->env_table = (t_env_table *)malloc(sizeof(t_env_table));
	if (!data->env_table)
		free_exit(data, "Error: malloc data->env_table failed\n");
	initialize_table(envp, data);
	initialize_envp_arr(data);
}