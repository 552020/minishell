/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 19:01:17 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/22 22:13:00 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	len;

	len = 0;
	while (*s)
	{
		s++;
		len++;
	}
	while (len != -1)
	{
		if (*s != (char)c)
		{
			s--;
			len--;
		}
		else
			return ((char *)s);
	}
	return (NULL);
}
/*
int main(void)
{
    char *test = "Berkcancox";
    char c = 'c';
    char *rest = ft_strrchr(test, c);
    printf("Rest of the string is: %s ",rest);
}
*/
