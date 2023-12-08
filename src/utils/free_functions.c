/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:43:13 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 06:43:16 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_arr(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->token_count + 1)
	{
		if (data->token_arr[i].str != NULL)
		{
			free(data->token_arr[i].str);
			data->token_arr[i].str = NULL;
		}
		i++;
	}
	if (data->token_arr)
	{
		free(data->token_arr);
		data->token_arr = NULL;
	}
}

void	free_lexeme_arr(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->token_count + 1)
	{
		if (data->lexeme_arr[i].str != NULL)
		{
			free(data->lexeme_arr[i].str);
			data->lexeme_arr[i].str = NULL;
			free(data->lexeme_arr[i].original);
			data->lexeme_arr[i].original = NULL;
		}
		i++;
	}
	free(data->lexeme_arr);
	data->lexeme_arr = NULL;
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
		free_token_arr(data);
	data->token_arr = NULL;
	if (data->lexeme_arr)
		free_lexeme_arr(data);
	data->lexeme_arr = NULL;
}

void	free_exit(t_data *data, char *error_message)
{
	perror(error_message);
	free_data(data);
	exit(FAILURE);
}
