/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:58:35 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 17:58:37 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_in_wrapper(size_t *i, size_t token_count, t_data *data)
{
	data->lexeme_arr[*i] = redirect_in_lexeme(&(data->token_arr[*i]), data);
	if (*i + 1 < token_count)
	{
		data->lexeme_arr[*i
			+ 1] = redirect_in_target_lexeme(&(data->token_arr[*i + 1]), data);
		(*i)++;
	}
}

void	redirect_out_wrapper(size_t *i, size_t token_count, t_data *data)
{
	data->lexeme_arr[*i] = redirect_out_lexeme(&(data->token_arr[*i]), data);
	if (*i < token_count - 1)
	{
		data->lexeme_arr[*i
			+ 1] = redirect_out_target_lexeme(&(data->token_arr[*i + 1]), data);
		(*i)++;
	}
}

void	redirect_append_wrapper(size_t *i, size_t token_count, t_data *data)
{
	data->lexeme_arr[*i] = redirect_append_lexeme(&(data->token_arr[*i]), data);
	if (*i < token_count - 1 && data->token_arr[*i + 1].type != T_END)
	{
		data->lexeme_arr[*i
			+ 1] = redirect_out_target_lexeme(&(data->token_arr[*i + 1]), data);
		(*i)++;
	}
}

void	heredoc_wrapper(t_lexeme *lexeme_arr, t_token *token_arr, size_t *i,
		t_data *data)
{
	lexeme_arr[*i] = heredoc_lexeme(&token_arr[*i], data);
	lexeme_arr[*i + 1] = heredoc_delimiter_lexeme(&token_arr[*i + 1], data);
	(*i)++;
}

void	undefined_wrapper(t_lexeme *lexeme_arr, t_token *token_arr, size_t *i,
		t_data *data)
{
	if (lexeme_arr[*i].type != L_FILENAME_STDIN
		|| lexeme_arr[*i].type != L_FILENAME_STDOUT)
		lexeme_arr[*i] = word_lexeme(&token_arr[*i], data);
}
