/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:34:43 by slombard          #+#    #+#             */
/*   Updated: 2022/12/21 17:38:07 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void	ft_write_n(char *str, unsigned int n, int len, int neg);
static int	ft_length_int(int n);

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	int		neg;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	neg = 0;
	len = ft_length_int(n);
	if (n < 0)
	{
		n *= -1;
		neg = 1;
	}
	str = malloc (sizeof(char) * len + 1);
	if (!str)
		return (0);
	if (n == 0)
	{
		str[1] = '\0';
		str[0] = '0';
	}
	if (n > 0)
		ft_write_n(str, n, len, neg);
	return (str);
}

static int	ft_length_int(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		n = -n;
		len = 1;
	}
	while (n >= 1)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_write_n(char *str, unsigned int n, int len, int neg)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (neg == 1)
	{
		str[0] = '-';
		i++;
	}
	while (i < len)
	{
		str[len - j - 1] = (n % 10) + '0';
		n = n / 10;
		i++;
		j++;
	}
	str[len] = '\0';
}
