/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:03:56 by slombard          #+#    #+#             */
/*   Updated: 2022/12/17 15:51:28 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*func)(unsigned int, char))
{
	int		i;
	size_t	strlen;
	char	*str;

	i = 0;
	strlen = ft_strlen((char *)s);
	str = malloc(sizeof(char) * (strlen + 1));
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		str[i] = func (i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
