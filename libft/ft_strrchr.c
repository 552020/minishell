/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 00:31:23 by slombard          #+#    #+#             */
/*   Updated: 2022/12/21 18:52:48 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	str_len;

	str_len = 0;
	while (s[str_len] != '\0')
		str_len++;
	while (str_len >= 0)
	{
		if ((char)c == s[str_len])
			return ((char *) &s[str_len]);
		str_len--;
	}
	return (NULL);
}
