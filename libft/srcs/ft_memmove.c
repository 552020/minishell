/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 17:37:34 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/16 20:07:30 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*destm;
	char	*srcm;
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (0);
	destm = (char *)dest;
	srcm = (char *)src;
	if (destm > srcm)
	{
		while (n--)
			destm[n] = srcm[n];
	}
	else
	{
		while (n--)
		{
			destm[i] = srcm[i];
			i++;
		}
	}
	return (destm);
}

/*
int main(void)
{
	char src[] = "Hello world!";
	char dest[] = "0101";
	
	memmove(dest, src, 5);
	printf("After memmove dest is :    %s, source is : %s\n", dest, src);
	printf("After memmove dest address is :   %p\n", &dest);
	ft_memmove(dest, src, 5);
	printf("After ft_memmove dest is : %s, source is : %s\n", dest, src);
	printf("After ft_memmove dest address is :%p\n", &dest);
	return 0;
}
*/
