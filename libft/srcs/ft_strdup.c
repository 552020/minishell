/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 17:21:33 by bsengeze          #+#    #+#             */
/*   Updated: 2022/12/24 17:10:54 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s1_mal;
	size_t	len;

	len = ft_strlen(s1);
	s1_mal = malloc(sizeof(char) * len + 1);
	if (!s1_mal)
		return (0);
	ft_strlcpy(s1_mal, s1, len + 1);
	return (s1_mal);
}
