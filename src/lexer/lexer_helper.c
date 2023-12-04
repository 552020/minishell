/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 18:59:11 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 18:59:14 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	finalize_lexeme_array(t_data *data, size_t i)
{
	data->lexeme_arr[i].type = L_END;
	data->lexeme_arr[i].str = NULL;
	command_and_args(data->token_count, data->lexeme_arr);
	if (data->token_arr[i].type == T_END)
		data->lexeme_arr[i].type = L_END;
}

void	lexer_t_var_subs(t_data *data, size_t i)
{
	if (data->token_arr[i].type == T_ENV_VAR)
		data->lexeme_arr[i] = t_env_var_subs(&data->token_arr[i], data);
	else if (data->token_arr[i].type == T_SHELL_VAR)
		data->lexeme_arr[i] = t_shell_var_subs(&data->token_arr[i], data);
}

/* We need the token variable for Norminette
The autoformatter otherwise mess up line splitting */
void	lexer_t_quotes_var_subs(t_data *data, size_t i)
{
	t_token	*token;

	token = &data->token_arr[i];
	if (data->token_arr[i].type == T_ENV_VAR
		|| data->token_arr[i].type == T_SHELL_VAR)
	{
		return ;
	}
	else
	{
		if (data->token_arr[i].type == T_DOUBLE_QUOTE)
			data->lexeme_arr[i] = t_double_quotes_var_subs(token, data);
		else if (data->token_arr[i].type == T_SINGLE_QUOTE)
			data->lexeme_arr[i] = single_quote_lexeme(token, data);
	}
}

void	lexer_t_pipe(t_data *data, size_t i)
{
	if (data->token_arr[i].type == T_ENV_VAR
		|| data->token_arr[i].type == T_SHELL_VAR
		|| data->token_arr[i].type == T_DOUBLE_QUOTE
		|| data->token_arr[i].type == T_SINGLE_QUOTE)
	{
		return ;
	}
	else if (data->token_arr[i].type == T_PIPE)
		data->lexeme_arr[i] = pipe_lexeme(&data->token_arr[i], data);
}

void	lexer_t_redirects_and_word(t_data *data, size_t i)
{
	if (data->token_arr[i].type == T_ENV_VAR
		|| data->token_arr[i].type == T_SHELL_VAR
		|| data->token_arr[i].type == T_DOUBLE_QUOTE
		|| data->token_arr[i].type == T_SINGLE_QUOTE
		|| data->token_arr[i].type == T_PIPE)
	{
		return ;
	}
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
}
