#include "minishell.h"

void initialize_free_data(t_free_data *free_data)
{
	free_data = (t_free_data *)malloc(sizeof(t_free_data));
	if (!free_data)
	{
		perror("malloc error\n");
		exit(FAILURE);
	}
	free_data->my_envp = NULL;
	free_data->env_table = NULL;
	free_data->ast_root = NULL;
}


void free_all_data(t_free_data *free_data)
{
    if (free_data->ast_root)
    {
        free_ast(free_data->ast_root);
        free_data->ast_root = NULL;
    }
    if (free_data->env_table)
    {
        free_hash_table(free_data->env_table);
        free_data->env_table = NULL;
    }
    if (free_data->my_envp)
    {
        free_envp(free_data->my_envp);
        free_data->my_envp = NULL;
    }
	free(free_data);
}
