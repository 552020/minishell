/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 20:54:30 by slombard          #+#    #+#             */
/*   Updated: 2022/12/17 00:03:12 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t b)
{
	size_t				i;
	const unsigned char	*ptr_s;

	i = 0;
	ptr_s = s;
	while (i < b)
	{
		if (*ptr_s == (unsigned char) c)
			return ((void *)ptr_s);
		ptr_s++;
		i++;
	}
	return (NULL);
}
