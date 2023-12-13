/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:19:07 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 07:48:41 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexeme_is_operator(t_lexeme_type type)
{
	if (type == L_PIPE || type == L_REDIRECT_INPUT || type == L_REDIRECT_OUTPUT
		|| type == L_REDIRECT_APPEND || type == L_HEREDOC || type == L_LOG_OR
		|| type == L_LOG_AND)
		return (1);
	return (0);
}

int	check_initial_pipe(t_data *data, size_t index)
{
	if (index == 0 && data->lexeme_arr[index].type == L_PIPE)
	{
		printf("Syntax error: unexpected token %s\n",
			data->lexeme_arr[index].str);
		free_lexeme_arr(data);
		return (1);
	}
	return (0);
}

int	check_unexpected_enf_of_input(t_data *data, size_t index)
{
	if (data->lexeme_arr[index + 1].type == L_END)
	{
		printf("Syntax error: unexpected end of input\n");
		free_lexeme_arr(data);
		return (1);
	}
	return (0);
}

int	check_unexpected_token(t_data *data, size_t index)
{
	if (data->lexeme_arr[index + 1].type == L_PIPE)
	{
		printf("Syntax error: unexpected token %s\n", data->lexeme_arr[index
			+ 1].str);
		free_lexeme_arr(data);
		return (1);
	}
	return (0);
}

int	check_syntax_error(t_data *data)
{
	size_t		i;
	t_lexeme	curr;

	curr = data->lexeme_arr[i];
	i = 0;
	while (i < data->token_count + 1)
	{
		curr = data->lexeme_arr[i];
		if (lexeme_is_operator(data->lexeme_arr[i].type))
		{
			if (check_initial_pipe(data, i))
				return (1);
			else if (check_unexpected_enf_of_input(data, i))
				return (1);
			else if (check_unexpected_token(data, i))
				return (1);
			else if (curr.type == L_PIPE || curr.type == L_LOG_OR
				|| curr.type == L_LOG_AND)
			{
				printf("Syntax error: unexpected token %s\n", data->lexeme_arr[i
					+ 1].str);
				free_lexeme_arr(data);
				return (1);
			}
		}
		i++;
	}
	return (0);
}
