/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 20:24:31 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/12 20:36:05 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
/*
#include <ctype.h>
#include <stdio.h>

int	main(void)
{
	printf("ft_isprint result is : %d \n", ft_isprint(50));
	printf("isprint result is : %d \n", isprint(50));
	printf("ft_isprint result is : %d \n", ft_isprint(10));
    printf("isprint result is : %d \n", isprint(10));
}
*/
