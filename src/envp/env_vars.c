/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:04:13 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:06:22 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_envp_split
{
	char	**ret;
	size_t	i;
	size_t	len;
}			t_envp_split;

void	init_t_envp_split(t_envp_split *split, const char *s, char c,
		t_data *data)
{
	split->ret = NULL;
	split->i = 0;
	split->len = 0;
	split->ret = (char **)malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!split->ret)
		free_exit(data, "Error: malloc failed\n");
}

void	entries_t_envp_split(t_envp_split *split, const char **s, char c,
		t_data *data)
{
	if (**s != c)
	{
		split->len = 0;
		while (**s && **s != c && ++split->len)
			++*s;
		split->ret[split->i] = ft_substr(*s - split->len, 0, split->len);
		if (!split->ret[split->i])
		{
			ft_free_ret(split->ret, split->i);
			free_exit(data, "Error: malloc failed\n");
		}
		split->i++;
	}
	else
	{
		if (*(*s + 1) == '\0')
			split->ret[split->i++] = ft_strdup("");
		(*s)++;
	}
}

char	**ft_split_envp(const char *s, char c, t_data *data)
{
	t_envp_split	split;

	init_t_envp_split(&split, s, c, data);
	while (*s)
		entries_t_envp_split(&split, &s, c, data);
	split.ret[split.i] = 0;
	return (split.ret);
}

void	initialize_table(char **envp, t_data *data)
{
	int		i;
	char	**key_value_pair;

	i = -1;
	while (++i < TABLE_SIZE)
		data->env_table->table[i] = NULL;
	data->env_table->count = 0;
	i = 0;
	while (envp[i] != NULL)
	{
		key_value_pair = ft_split_envp(envp[i], '=', data);
		if (!key_value_pair)
			free_exit(data, "Error: ft_split in init table failed\n");
		if (!key_value_pair[0])
			free_exit(data, "Error: ft_split in init table failed\n");
		data->env_table = data->env_table;
		insert_node_ht(key_value_pair[0], key_value_pair[1], data);
		data->env_table->count++;
		free_key_value_pair(key_value_pair);
		i++;
	}
}
