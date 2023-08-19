/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:53:32 by slombard          #+#    #+#             */
/*   Updated: 2022/12/16 15:49:34 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;
	size_t	i;

	i = 0;
	ptr = malloc(size * nmemb);
	if (!ptr)
	{
		return (NULL);
	}		
	while (i < nmemb * size)
	{
		ptr[i] = '\0';
		i++;
	}
	return ((void *) ptr);
}
