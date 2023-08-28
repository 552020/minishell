/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 19:33:15 by bsengeze          #+#    #+#             */
/*   Updated: 2023/07/04 04:30:13 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// strncmp function lexicographically compare the null-terminated strings s1
// and s2. The strncmp() function compares not more than n characters. Because
// strncmp is designed forcomparing strings rather than binary data, characters
// that appear after a ‘\0’ character are notcompared.
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (n > 0)
	{
		if (s1[i] != s2[i] || !s1[i] || !s2[i])
			return ((unsigned char)s1[i] -(unsigned char)s2[i]);
		if (s1[i] == s2[i])
			i++;
		n--;
	}
	return (0);
}
/*
int main(void)
{
	printf("result : %d\n", ft_strncmp("test\200", "test\0", 6));
	printf("result : %d", strncmp("test\200", "test\0", 6));
}
*/
