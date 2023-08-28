/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 00:40:22 by bsengeze          #+#    #+#             */
/*   Updated: 2023/07/26 21:50:39 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Locates the first occurrence of the null-terminated string little in the 
// big string, where not more than len characters are searched.  Characters 
// that appear after a ‘\0’ character are not searched.
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*little)
		return ((char *)big);
	while (big[i] != 0)
	{
		j = 0;
		while (little[j] == big[i + j] && len > i + j)
		{
			if (little[j + 1] == 0)
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (0);
}
/*
int main(void)
{
	printf("my func result %s\n", ft_strnstr("lorem ipsum dolor","ipsumm",6));
    //printf("lib func result %s", strnstr("lorem ipsum","ipsumm",6));
}
*/
