/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:32:15 by slombard          #+#    #+#             */
/*   Updated: 2022/12/17 22:19:39 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	substr_len;
	int		s_len;

	s_len = ft_strlen((char *)s);
	if (start >= ft_strlen((char *)s))
	{
		substr = malloc (sizeof(char));
		if (!substr)
			return (NULL);
		*substr = '\0';
		return (substr);
	}
	if (len <= s_len - start)
		substr_len = len;
	else
		substr_len = s_len - start;
	substr = malloc (sizeof(char) * substr_len + 1);
	if (substr == NULL)
		return (0);
	ft_strlcpy(substr, &s[start], substr_len + 1);
	return (&substr[0]);
}
