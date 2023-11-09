#include "minishell.h"

void	initialize_data(t_data *data)
{
	data->env_arr = NULL;
	data->env_table = NULL;
	data->token_arr = NULL;
	data->lexeme_arr = NULL;
	data->ast_root = NULL;
}