/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexemes_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:20:12 by slombard          #+#    #+#             */
/*   Updated: 2023/12/13 14:20:14 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexeme	log_or_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_LOG_OR;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	log_and_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_LOG_AND;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	parentheses_open_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_PARENTHESIS_OPEN;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}
t_lexeme	parentheses_close_lexeme(t_token *token, t_data *data)
{
	t_lexeme lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_PARENTHESIS_CLOSED;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}