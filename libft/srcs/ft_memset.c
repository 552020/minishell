/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 21:24:24 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/13 16:07:32 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = s;
	i = 0;
	while (n > i)
	{
		str [i] = c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	*str;

	str = strdup("helloooooo world");
	if (!ft_memset((void *)str, 'A', 5))
	{
		printf("An error occurred in ft_memset()\n");
	}
	else
	{
		printf("%s\n", str);
	}
}
*/
