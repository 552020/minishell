/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:06:35 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:06:51 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_env_var **table, const char *key)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = table[idx];
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			return (node->value);
		}
		node = node->next;
	}
	return (NULL);
}

char	**hash_table_to_arr(t_data *data)
{
	int			i;
	int			j;
	char		**envp;
	char		*temp;
	t_env_var	*node;

	if (data->env_table->count == 0)
		return (NULL);
	if (data->env_arr != NULL)
		free_envp(data->env_arr);
	envp = (char **)malloc(sizeof(char *) * (data->env_table->count + 1));
	if (envp == NULL)
		free_exit(data, "Error: malloc in hash_table_to_arr failed\n");
	data->env_arr = envp;
	i = 0;
	j = 0;
	while (i < TABLE_SIZE)
	{
		node = data->env_table->table[i];
		while (node != NULL)
		{
			temp = ft_strjoin(node->key, "=");
			if (!temp)
				free_exit(data, "Error: ft_strjoin failed\n");
			envp[j] = ft_strjoin(temp, node->value);
			if (!envp[j])
			{
				free(temp);
				temp = NULL;
				free_exit(data, "Error: ft_strjoin failed\n");
			}
			free(temp);
			temp = NULL;
			j++;
			node = node->next;
		}
		i++;
	}
	envp[j] = NULL;
	return (envp);
}
