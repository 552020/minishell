/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reshuffle_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:03:55 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 06:19:01 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_unpaired_quote(const char *str)
{
	const char	*current;
	char		*next_quote;

	if (!str)
		return (NULL);
	current = str;
	current = ft_strchr(current, '\'');
	while ((current))
	{
		next_quote = ft_strchr(current + 1, '\'');
		if (!next_quote)
			return ((char *)current);
		else
			current = next_quote + 1;
		current = ft_strchr(current, '\'');
	}
	return (NULL);
}

int	count_single_or_double_quotes(const char *str, char quote)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == quote)
			count++;
		str++;
	}
	return (count);
}

char	*remove_single_or_double_quotes(char *str, char quote)
{
	char	*dest;
	char	*ret;
	char	*unpaired_quote;
	int		str_len;
	int		quotes_count;

	if (!str)
		return (NULL);
	quotes_count = count_single_or_double_quotes(str, quote);
	str_len = ft_strlen(str) - quotes_count;
	dest = ft_calloc(str_len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	ret = dest;
	unpaired_quote = find_unpaired_quote(str);
	while (*str)
	{
		if (*str != quote || (str == unpaired_quote))
			*dest++ = *str;
		str++;
	}
	return (ret);
}

char	*add_single_or_double_quotes(char *str, char quote)
{
	char	*result;
	char	*start;
	int		str_len;

	str_len = ft_strlen(str);
	result = ft_calloc(str_len + 3, sizeof(char));
	start = result;
	if (!result)
		return (NULL);
	*result++ = quote;
	while (*str)
		*result++ = *str++;
	*result++ = quote;
	return (start);
}
