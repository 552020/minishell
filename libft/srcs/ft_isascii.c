/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 20:17:22 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/12 20:28:52 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/*
#include <ctype.h>
#include <stdio.h>

int	main(void)
{
	printf("ft_isascii result is : %d \n", ft_isascii(50));
	printf("isascii result is : %d \n", isascii(50));
	printf("ft_isascii result is : %d \n", ft_isascii(128));
    printf("isascii result is : %d \n", isascii(128));
}*/
