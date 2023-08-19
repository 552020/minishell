/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 18:32:36 by slombard          #+#    #+#             */
/*   Updated: 2022/12/22 19:55:18 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*ptr_d;
	const char	*ptr_s;
	size_t		i;

	if (!dest && !src)
		return (NULL);
	ptr_d = dest;
	ptr_s = src;
	i = 0;
	while (i < n)
	{
		*ptr_d = *ptr_s;
		ptr_s++;
		ptr_d++;
		i++;
	}
	return (dest);
}
