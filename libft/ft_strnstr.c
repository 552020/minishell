/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 17:54:21 by slombard          #+#    #+#             */
/*   Updated: 2022/12/16 19:59:30 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lit, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (lit[0] == '\0')
		return ((char *)big);
	while (big[i] && (i < len))
	{
		if (big[i] == lit[j])
		{
			while ((big[i + j] == lit[j] || lit[j] == '\0') && (i + j) <= len)
			{
				if (!lit[j])
					return ((char *) &big[i]);
				j++;
			}	
		}
		j = 0;
		i++;
	}
	return (NULL);
}
