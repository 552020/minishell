#include "minishell.h"

void	initialize_data(t_data *data, t_env_table *env_table)
{
	data->my_envp = NULL;
	data->env_table = env_table;
	data->ast_root = NULL;
}

void	free_all_data(t_data *data)
{
	if (data->ast_root)
		free_ast(data->ast_root);
	if (data->env_table)
		free_hash_table(data->env_table);
	if (data->my_envp)
		free_envp(data->my_envp);
	free(data);
}

void	free_exit(t_data *data, char *error_message)
{
	perror(error_message);
	// rl_clear_history();
	free_all_data(data);
	exit(FAILURE);
}