/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reshuffle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 05:47:17 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 06:18:55 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct reshuffle_quotes
{
	const char	*start_input;
	const char	*start_sub;
	char		*end_sub;
	char		*sub;
	char		*tmp;
	char		*before;
	char		*after;
	char		*result;
	char		*cur;
	int			before_and_sub_len;
}				t_reshuffle_quotes;

void	build_substring(t_reshuffle_quotes *rsq, char quote)
{
	rsq->start_sub = rsq->cur;
	rsq->end_sub = ft_strchr(rsq->cur + 1, *rsq->cur);
	while (rsq->start_sub > rsq->start_input && isregularchar(*(rsq->start_sub
				- 1), rsq->cur))
	{
		rsq->start_sub--;
	}
	while (*rsq->end_sub && (isregularchar(*rsq->end_sub, rsq->end_sub)
			|| *rsq->end_sub == quote))
		rsq->end_sub++;
	rsq->tmp = ft_substr(rsq->start_sub, 0, rsq->end_sub - rsq->start_sub);
}

void	remove_add_single_or_double_quotes(t_reshuffle_quotes *rsq, char quote)
{
	rsq->sub = remove_single_or_double_quotes(rsq->tmp, quote);
	free(rsq->tmp);
	rsq->tmp = add_single_or_double_quotes(rsq->sub, quote);
	free(rsq->sub);
}

void	build_result(t_reshuffle_quotes *rsq)
{
	rsq->sub = rsq->tmp;
	rsq->before = ft_substr(rsq->result, 0, rsq->start_sub - rsq->start_input);
	rsq->after = ft_strdup(rsq->end_sub);
	free(rsq->result);
	rsq->result = ft_strjoin(rsq->before, rsq->sub);
	rsq->before_and_sub_len = ft_strlen(rsq->result);
	free(rsq->before);
	free(rsq->sub);
	rsq->tmp = ft_strjoin(rsq->result, rsq->after);
	free(rsq->result);
	free(rsq->after);
	rsq->result = rsq->tmp;
	rsq->cur = rsq->result + rsq->before_and_sub_len;
}

char	*reshuffle_double_quotes(const char *input)
{
	t_reshuffle_quotes	rsq;

	if (!input)
		return (NULL);
	rsq.result = ft_strdup(input);
	rsq.cur = rsq.result;
	rsq.start_input = rsq.result;
	while (*rsq.cur)
	{
		rsq.start_input = rsq.result;
		if (*rsq.cur == '"')
		{
			if (isregularchar(*rsq.cur, rsq.cur))
				break ;
			build_substring(&rsq, '"');
			remove_add_single_or_double_quotes(&rsq, '"');
			build_result(&rsq);
		}
		if (*rsq.cur)
			rsq.cur++;
	}
	return (rsq.result);
}

char	*reshuffle_single_quotes(const char *input)
{
	t_reshuffle_quotes	rsq;

	if (!input)
		return (NULL);
	rsq.result = ft_strdup(input);
	rsq.cur = rsq.result;
	rsq.start_input = rsq.result;
	while (*rsq.cur)
	{
		rsq.start_input = rsq.result;
		if (*rsq.cur == '\'')
		{
			if (isregularchar(*rsq.cur, rsq.cur))
				break ;
			build_substring(&rsq, '\'');
			remove_add_single_or_double_quotes(&rsq, '\'');
			build_result(&rsq);
		}
		if (*rsq.cur)
			rsq.cur++;
	}
	return (rsq.result);
}
