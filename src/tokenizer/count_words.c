/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 05:35:29 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 05:43:38 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_dollar_sign(const char **str_ptr)
{
	(*str_ptr)++;
	if (**str_ptr == '?')
		(*str_ptr)++;
	else
	{
		while (**str_ptr && ft_isvalidvarname(**str_ptr))
			(*str_ptr)++;
	}
}

void	parse_redirect(const char **str_ptr)
{
	(*str_ptr)++;
	if (**str_ptr == *(*str_ptr - 1))
		(*str_ptr)++;
}

void	parse_quotes(const char **str_ptr)
{
	char	quote;

	quote = **str_ptr;
	(*str_ptr)++;
	while (**str_ptr && **str_ptr != quote)
		(*str_ptr)++;
	if (**str_ptr)
		(*str_ptr)++;
}

void	count_word_special_char(const char **str_ptr, size_t *words)
{
	if (isspecialchar(**str_ptr, *str_ptr))
	{
		(*words)++;
		if (**str_ptr == '$')
			parse_dollar_sign(str_ptr);
		else if ((**str_ptr == '<' || **str_ptr == '>'))
			parse_redirect(str_ptr);
		else if (**str_ptr == '\'' || **str_ptr == '"')
			parse_quotes(str_ptr);
		else
		{
			if (**str_ptr)
				(*str_ptr)++;
		}
	}
}

size_t	count_words_tokenizer(const char *str)
{
	size_t	words;

	words = 0;
	while (*str)
	{
		skip_spaces(&str);
		if (*str == '\0')
			break ;
		if (isspecialchar(*str, str))
			count_word_special_char(&str, &words);
		else if (isregularchar(*str, str))
		{
			words++;
			while (*str && isregularchar(*str, str))
				str++;
		}
		else
		{
			ft_putendl_fd("Warning: Unexpected char.", STDERR_FILENO);
			str++;
		}
	}
	return (words);
}
