/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 23:10:44 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/19 01:32:13 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*cstr;
	unsigned char	cc;

	cstr = (unsigned char *)s;
	cc = (unsigned char)c;
	while (n > 0)
	{
		if (*cstr == cc)
			return (cstr);
		cstr++;
		n--;
	}
	return (0);
}
/*
int main(void)
{
	printf("my funct result is : %p", ft_memchr("ABCD", 'A', 5));
    printf("lib funct result is : %p", memchr("ABCD", 'A', 5));

}
*/
