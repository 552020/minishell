/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 19:50:40 by slombard          #+#    #+#             */
/*   Updated: 2022/12/21 18:34:22 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"  

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int		i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] && s2[i] && i < n - 1)
	{
		if (s1[i] != s2[i])
			return ((unsigned char )s1[i] - (unsigned char )s2[i]);
		i++;
	}
	return ((unsigned char )s1[i] - (unsigned char )s2[i]);
}
