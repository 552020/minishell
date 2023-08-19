/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 15:30:34 by slombard          #+#    #+#             */
/*   Updated: 2022/12/21 17:25:57 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

void	ft_putnbr_fd(int n, int fd)
{
	int		i;

	if (n == -2147483648)
	{
		ft_putstr_fd("-2", fd);
		n = 147483648;
	}
	if (n < 0 && n != -2147483648)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		ft_putnbr_fd (n / 10, fd);
		i = n % 10;
		ft_putchar_fd('0' + i, fd);
	}
	if (n < 10)
	{
		i = n % 10;
		ft_putchar_fd('0' + i, fd);
	}
}
/*
int main(void)
{
	ft_putstr_fd("-2147483648", 1);
	return (0);
}
*/
