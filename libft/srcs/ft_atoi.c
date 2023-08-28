/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:41:14 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/20 17:17:56 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		result;
	int		neg;

	result = 0;
	neg = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr++ == '-')
			neg *= -1;
	}
	while (ft_isdigit(*nptr))
		result = result * 10 + neg * (*nptr++ - '0');
	return (result);
}
/*
int main(void)
{
	printf("my atoi : %d", ft_atoi("-123"));
    printf("lib atoi : %d", atoi("-123"));

}
*/
