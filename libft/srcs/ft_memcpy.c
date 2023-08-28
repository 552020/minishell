/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 17:09:39 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/16 18:01:50 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (0);
	while (n > i)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*src;
	char	*dest;

	src = strdup("hello world");
	dest = strdup("heyyy");
	if (!ft_memcpy((void *)dest, (const void *)src, 3))
	{
		printf("An error occurred in ft_memcpy()\n");
	}
	else
	{
		printf("source is : %s\n dest is : %s\n", src, dest);
	}
}
*/
