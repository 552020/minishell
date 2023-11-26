#include "minishell.h"

void	free_token_arr(t_token *token_arr)
{
	size_t	i;

	if (token_arr == NULL)
		return ;
	i = 0;
	while (token_arr[i].type != T_END)
	{
		if (token_arr[i].str != NULL)
		{
			free(token_arr[i].str);
			token_arr[i].str = NULL;
		}
		i++;
	}
	free(token_arr);
	token_arr = NULL;
}

void	free_lexeme_arr(t_lexeme *lexeme_arr)
{
	size_t	i;

	i = 0;
	while (lexeme_arr[i].type != L_END)
	{
		if (lexeme_arr[i].str != NULL)
		{
			free(lexeme_arr[i].str);
			lexeme_arr[i].str = NULL;
			free(lexeme_arr[i].original);
			lexeme_arr[i].original = NULL;
		}
		i++;
	}
	free(lexeme_arr);
	lexeme_arr = NULL;
}

void	free_key_value_pair(char **key_value)
{
	int	i;

	i = 0;
	while (key_value[i] != NULL)
	{
		free(key_value[i]);
		key_value[i] = NULL;
		i++;
	}
	free(key_value);
	key_value = NULL;
}

void	free_data(t_data *data)
{
	if (data->env_table)
		free_hash_table(data->env_table);
	data->env_table = NULL;
	if (data->env_arr)
		free_envp(data->env_arr);
	data->env_arr = NULL;
	if (data->ast_root)
		free_ast(data->ast_root);
	data->ast_root = NULL;
	if (data->token_arr)
		free_token_arr(data->token_arr);
	data->token_arr = NULL;
	if (data->lexeme_arr)
		free_lexeme_arr(data->lexeme_arr);
	data->lexeme_arr = NULL;
}

void	free_exit(t_data *data, char *error_message)
{
	// to do: make it take another argument exit status and return exit status
	perror(error_message);
	free_data(data);
	exit(FAILURE);
}
