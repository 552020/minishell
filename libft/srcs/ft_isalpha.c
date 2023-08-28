/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 16:55:17 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/12 19:11:08 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
printf("ft_isalpha result is : %d \n", ft_isalpha(75));
printf("isalpha result is : %d \n", isalpha(75));
}
*/
