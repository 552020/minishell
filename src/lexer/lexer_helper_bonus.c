/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:29:09 by slombard          #+#    #+#             */
/*   Updated: 2023/12/13 14:29:11 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_t_log_and_or(t_data *data, size_t i)
{
	if (data->token_arr[i].type == T_ENV_VAR
		|| data->token_arr[i].type == T_SHELL_VAR
		|| data->token_arr[i].type == T_DOUBLE_QUOTE
		|| data->token_arr[i].type == T_SINGLE_QUOTE
		|| data->token_arr[i].type == T_PIPE)
	{
		return ;
	}
	else if (data->token_arr[i].type == T_LOG_OR)
		data->lexeme_arr[i] = log_or_lexeme(&data->token_arr[i], data);
	else if (data->token_arr[i].type == T_LOG_AND)
		data->lexeme_arr[i] = log_and_lexeme(&data->token_arr[i], data);
}

void	lexer_t_parentheses(t_data *data, size_t i)
{
	if (data->token_arr[i].type == T_ENV_VAR
		|| data->token_arr[i].type == T_SHELL_VAR
		|| data->token_arr[i].type == T_DOUBLE_QUOTE
		|| data->token_arr[i].type == T_SINGLE_QUOTE
		|| data->token_arr[i].type == T_PIPE
		|| data->token_arr[i].type == T_LOG_OR
		|| data->token_arr[i].type == T_LOG_AND)
	{
		return ;
	}
	else if (data->token_arr[i].type == T_PARENTHESIS_OPEN)
		data->lexeme_arr[i] = parentheses_open_lexeme(&data->token_arr[i],
			data);
	else if (data->token_arr[i].type == T_PARENTHESIS_CLOSE)
		data->lexeme_arr[i] = parentheses_close_lexeme(&data->token_arr[i],
			data);
}