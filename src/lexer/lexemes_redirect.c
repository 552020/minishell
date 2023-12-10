/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexemes_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:10:02 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 07:10:30 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexeme	redirect_in_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_INPUT;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

char	*assign_redirect_target(t_token *token, t_data *data)
{
	char	*target;
	char	*tmp;

	if (token->type == T_DOUBLE_QUOTE)
	{
		tmp = process_vars_in_str(token->str, data);
		if (!tmp)
			free_exit(data, "Error: malloc target failed\n");
		target = strip_quotes(tmp, data);
		free(tmp);
	}
	else if (token->type == T_SINGLE_QUOTE)
		target = strip_quotes(token->str, data);
	else
		target = ft_strdup(token->str);
	if (!target)
		free_exit(data, "Error: malloc target failed\n");
	return (target);
}

t_lexeme	redirect_in_target_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDIN;
	lexeme.str = assign_redirect_target(token, data);
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_OUTPUT;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_target_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDOUT;
	lexeme.str = assign_redirect_target(token, data);
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}
