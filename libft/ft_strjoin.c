/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 19:22:19 by slombard          #+#    #+#             */
/*   Updated: 2022/12/22 19:36:59 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	strlen_s1;
	size_t	strlen_s2;
	char	*res;

	strlen_s1 = ft_strlen(s1);
	strlen_s2 = ft_strlen(s2);
	res = malloc (sizeof (char) * (strlen_s1 + strlen_s2 + 1));
	if (!res)
		return (NULL);
	ft_strlcpy (res, s1, (strlen_s1 + 1));
	ft_strlcat (res, s2, (strlen_s1 + strlen_s2 + 1));
	return (res);
}
