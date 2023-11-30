/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:12:29 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 07:15:01 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_lexeme_arr(t_data *data)
{
	data->lexeme_arr = malloc(sizeof(t_lexeme) * (data->token_count + 1));
	if (!data->lexeme_arr)
		free_exit(data, "Error: malloc lexeme_arr failed\n");
	ft_memset(data->lexeme_arr, 0, sizeof(t_lexeme) * (data->token_count + 1));
}

void	command_and_args(size_t token_count, t_lexeme *lexeme_arr)
{
	size_t	i;
	int		command_flag;

	i = 0;
	command_flag = NO_CMD_YET;
	while (i < token_count)
	{
		if (lexeme_arr[i].type == L_UNDEFINED)
		{
			if (command_flag == NO_CMD_YET)
			{
				lexeme_arr[i].type = L_COMMAND;
				command_flag = CMD_FOUND;
			}
			else
				lexeme_arr[i].type = L_ARGUMENT;
		}
		else if (lexeme_arr[i].type == L_PIPE)
			command_flag = NO_CMD_YET;
		i++;
	}
}

t_lexeme	*lexer(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->token_count)
	{
		if (data->token_arr[i].type == T_ENV_VAR)
			data->lexeme_arr[i] = t_env_var_subs(&data->token_arr[i], data);
		else if (data->token_arr[i].type == T_SHELL_VAR)
			data->lexeme_arr[i] = t_shell_var_subs(&data->token_arr[i], data);
		else if (data->token_arr[i].type == T_DOUBLE_QUOTE)
			data->lexeme_arr[i] = t_double_quotes_var_subs(&data->token_arr[i],
					data);
		else if (data->token_arr[i].type == T_SINGLE_QUOTE)
			data->lexeme_arr[i] = single_quote_lexeme(&data->token_arr[i],
					data);
		else if (data->token_arr[i].type == T_PIPE)
			data->lexeme_arr[i] = pipe_lexeme(&data->token_arr[i], data);
		else if (data->token_arr[i].type == T_REDIRECT_IN)
			redirect_in_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_REDIRECT_OUT)
			redirect_out_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_REDIRECT_APPEND)
			redirect_append_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_HEREDOC)
			heredoc_wrapper(data->lexeme_arr, data->token_arr, &i, data);
		else if (data->token_arr[i].type == T_WORD)
			undefined_wrapper(data->lexeme_arr, data->token_arr, &i, data);
		else
			continue ;
		i++;
	}
	data->lexeme_arr[i].type = L_END;
	data->lexeme_arr[i].str = NULL;
	command_and_args(data->token_count, data->lexeme_arr);
	if (data->token_arr[i].type == T_END)
		data->lexeme_arr[i].type = L_END;
	return (data->lexeme_arr);
}

int	lexemize(t_data *data)
{
	create_lexeme_arr(data);
	data->lexeme_arr = lexer(data);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_LEXER)
	{
		printf("\n***Lexer***\n\n");
		print_lexeme_arr(data->lexeme_arr, data->token_count);
	}
	if (data->token_arr)
		free_token_arr(data);
	if (check_syntax_error(data))
		return (FAILURE);
	return (SUCCESS);
}
