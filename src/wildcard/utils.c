/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 04:01:39 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 04:19:40 by slombard         ###   ########.fr       */
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
		free_exit(data, "Malloc failed");
	ret[size] = NULL;
	i = 0;
	while (i < size)
	{
		ret[i] = matching[i]->entry;
		i++;
	}
	return (ret);
}

typedef struct ft_strjoin_arr
{
	char		*ret;
	int			idx;
	int			len;
}				t_ft_strjoin_arr;

void	init_ft_strjoin_arr(t_ft_strjoin_arr *vars)
{
	vars->ret = NULL;
	vars->idx = 0;
	vars->len = 0;
}

char	*ft_strjoin_arr(char **arr)
{
	t_ft_strjoin_arr	vars;

	init_ft_strjoin_arr(&vars);
	if (!arr)
		return (ft_strdup(""));
	if (!arr[0])
		return (ft_strdup(""));
	while (arr[vars.idx])
	{
		vars.len += ft_strlen(arr[vars.idx]);
		vars.idx++;
	}
	vars.ret = malloc(sizeof(char) * (vars.len + vars.idx + 1));
	if (!vars.ret)
		return (NULL);
	ft_strlcpy(vars.ret, arr[0], vars.len + vars.idx);
	vars.idx = 1;
	while (arr[vars.idx])
	{
		ft_strlcat(vars.ret, " ", vars.len + vars.idx + 1);
		ft_strlcat(vars.ret, arr[vars.idx], vars.len + vars.idx + 1);
		vars.idx++;
	}
	return (vars.ret);
}

typedef struct s_reduce_consecutive_char
{
	char		*ret;
	const char	*src_ptr;
	char		*dest_ptr;
}				t_reduce_consecutive_char;

char	*reduce_consecutive_char(const char *str, char c, t_data *data)
{
	t_reduce_consecutive_char	vars;

	if (str == NULL)
		return (NULL);
	vars.ret = malloc(ft_strlen(str) + 1);
	if (!vars.ret)
		free_exit(data, "Malloc failed");
	vars.src_ptr = str;
	vars.dest_ptr = vars.ret;
	while (*vars.src_ptr)
	{
		*vars.dest_ptr = *vars.src_ptr;
		if (*vars.src_ptr == c)
		{
			while (*vars.src_ptr == c)
				vars.src_ptr++;
		}
		else
			vars.src_ptr++;
		vars.dest_ptr++;
	}
	*vars.dest_ptr = '\0';
	return (vars.ret);
}

void	check_prefix_midfixes_suffix(t_entries *entries, t_pattern *pattern)
{
	if (pattern->prefix_len > 0)
		check_prefix(entries, pattern);
	if (pattern->midfixes_nbr > 0)
		check_midfixes(entries, pattern);
	if (pattern->suffix_len > 0)
		check_suffix(entries, pattern);
}
