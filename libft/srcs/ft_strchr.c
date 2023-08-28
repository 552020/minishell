/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 18:18:24 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/22 22:10:33 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}
/*
int main(void)
{
    char *test = "Berkcan";
    char c = 'c';
    //char *rest = ft_strchr(test, c);
    //printf("Rest of the string is: %s ",rest);
    printf("Result is: %s ",ft_strchr(test, c));

}
*/
