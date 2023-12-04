/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:46:41 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 21:42:19 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

// static size_t	ft_count_word(const char *s, char c)
//{
//	size_t	i;
//	size_t	words;

//	i = 0;
//	words = 0;
//	if (!s[0])
//		return (0);
//	while (s[i] && s[i] == c)
//		i++;
//	while (s[i] != '\0')
//	{
//		if (s[i] == c)
//		{
//			words++;
//			while (s[i] && s[i] == c)
//				i++;
//		}
//		else
//			i++;
//	}
//	if (s[i - 1] != c)
//		words++;
//	return (words);
//}
