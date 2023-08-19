/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 21:02:55 by slombard          #+#    #+#             */
/*   Updated: 2022/12/16 23:51:55 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*ptr_d;
	const char	*ptr_s;
	char		*ptr_db;
	const char	*ptr_sb;

	ptr_d = dest;
	ptr_s = src;
	if (ptr_d < ptr_s)
	{
		while (n--)
			*ptr_d++ = *ptr_s++;
		return (dest);
	}
	else if (ptr_d > ptr_s)
	{
		ptr_db = dest + (n - 1);
		ptr_sb = src + (n - 1);
		while (n--)
			*ptr_db-- = *ptr_sb--;
		return (dest);
	}
	return (dest);
}
