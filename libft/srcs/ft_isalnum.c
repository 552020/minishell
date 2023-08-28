/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 20:03:59 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/12 20:29:48 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
printf("ft_isalnum result is : %d \n", ft_isalnum(75));
printf("isalnum result is : %d \n", isalnum(75));
printf("ft_isalnum result is : %d \n", ft_isalnum(50));
printf("isalnum result is : %d \n", isalnum(50));
printf("ft_isalnum result is : %d \n", ft_isalnum(1));
printf("isalnum result is : %d \n", isalnum(1));
}
*/
