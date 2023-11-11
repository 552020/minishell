/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 21:05:52 by slombard          #+#    #+#             */
/*   Updated: 2022/12/22 21:06:52 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	char	*index_start;
	char	*index_end;
	size_t	s1_len;

	if (!s1 || !set)
		return (NULL);
	s1_len = ft_strlen(s1);
	index_start = (char *)s1;
	index_end = index_start + s1_len - 1;
	while (*index_start && ft_check(*index_start, set))
		index_start++;
	while (index_end > index_start && ft_check(*index_end, set))
		index_end--;
	res = malloc(index_end - index_start + 2);
	if (!res)
		return (NULL);
	ft_strlcpy(res, index_start, index_end - index_start + 2);
	return (res);
}
