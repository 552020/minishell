#include "minishell.h"

void	free_data(t_data *data)
{
	if (data->env_table)
	{
		// free_hash_table(data->env_table->table);
		free_hash_table(data->env_table);
		free(data->env_table);
	}
	if (data->env_arr)
		free_envp(data->env_arr);
	if (data->ast_root)
		free_ast(data->ast_root);
	if (data->token_arr)
		free_token_arr(data->token_arr);
	if (data->lexeme_arr)
		free_lexeme_arr(data->lexeme_arr);
}

void	free_exit(t_data *data, char *error_message)
{
	perror(error_message);
	free_data(data);
	exit(FAILURE);
}