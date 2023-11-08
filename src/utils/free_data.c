#include "minishell.h"

void initialize_free_data(t_free_data *free_data, t_env_table *env_table)
{
	free_data->my_envp = NULL;
	free_data->env_table = env_table;
	free_data->ast_root = NULL;
}


void free_all_data(t_free_data *free_data)
{
    if (free_data->ast_root)
        free_ast(free_data->ast_root);
    if (free_data->env_table)
        free_hash_table(free_data->env_table);
    if (free_data->my_envp)
        free_envp(free_data->my_envp);
	free(free_data);
}

void free_exit(t_free_data *free_data, char *error_message)
{
	perror(error_message);
	// rl_clear_history();
	free_all_data(free_data);
	exit(FAILURE);
}