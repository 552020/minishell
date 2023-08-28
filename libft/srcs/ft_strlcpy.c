/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 20:11:00 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/18 16:00:00 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	i = 0;
	if (size == 0)
		return (len);
	if (size > 0)
	{
		while (size -1 > i && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len);
}
/*
int main()
{
	char dst[] = "1234";
	char src[] = "ABCD";

	ft_strlcpy(dst, src, 2);
	printf("After ft dest is: %s\n, len is : %d\n", dst, ft_strlcpy(dst, src, 2));

}
*/