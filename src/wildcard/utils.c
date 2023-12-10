/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 04:01:39 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 04:02:57 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

char	**entry_to_char(t_entry **matching, t_data *data)
{
	char	**ret;
	int		size;
	int		i;

	size = 0;
	while (matching[size])
		size++;
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
	{
		free_exit(data, "Malloc failed");
		return (NULL);
	}
	ret[size] = NULL;
	i = 0;
	while (i < size)
	{
		ret[i] = matching[i]->entry;
		i++;
	}
	return (ret);
}

char	*ft_strjoin_arr(char **arr)
{
	char	*ret;
	int		idx;
	int		len;

	idx = 0;
	len = 0;
	if (!arr)
		return (ft_strdup(""));
	if (!arr[0])
		return (ft_strdup(""));
	while (arr[idx])
	{
		len += ft_strlen(arr[idx]);
		idx++;
	}
	ret = malloc(sizeof(char) * (len + idx + 1));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, arr[0], len + idx);
	idx = 1;
	while (arr[idx])
	{
		ft_strlcat(ret, " ", len + idx + 1);
		ft_strlcat(ret, arr[idx], len + idx + 1);
		idx++;
	}
	return (ret);
}

char	*reduce_consecutive_char(const char *str, char c, t_data *data)
{
	char *ret;
	const char *src_ptr;
	char *dest_ptr;

	if (str == NULL)
		return (NULL);
	ret = malloc(ft_strlen(str) + 1);
	if (!ret)
	{
		free_exit(data, "Malloc failed");
		return (NULL);
	}
	src_ptr = str;
	dest_ptr = ret;
	while (*src_ptr)
	{
		*dest_ptr = *src_ptr;
		if (*src_ptr == c)
		{
			while (*src_ptr == c)
				src_ptr++;
		}
		else
			src_ptr++;
		dest_ptr++;
	}
	*dest_ptr = '\0';
	return (ret);
}