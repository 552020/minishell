/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 16:01:52 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/18 17:43:11 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	j;
	size_t	src_len;

	i = 0;
	j = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size == 0 || size <= dst_len)
		return (src_len + size);
	while (dst[i])
		i++;
	while (src[j] && (size - dst_len - 1) > j)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (src_len + dst_len);
}
/*
int main(void)
{
    char dst[] = "1234";
	char src[] = "ABCD";

	ft_strlcat(dst, src, 8);
	printf("After ft dest is: %s\n, len is : %d\n", dst, ft_strlcat(dst, src, 8));
}
*/
