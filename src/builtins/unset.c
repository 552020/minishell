/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:07:44 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 23:09:28 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_key_value_and_node(t_env_var *node)
{
	if (node->key)
	{
		free(node->key);
		node->key = NULL;
	}
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	if (node)
	{
		free(node);
		node = NULL;
	}
}

void	single_unset(const char *key, t_data *data)
{
	unsigned int	idx;
	t_env_var		*node;
	t_env_var		*prev;

	idx = hash(key);
	node = data->env_table->table[idx];
	prev = NULL;
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			if (prev == NULL)
				data->env_table->table[idx] = node->next;
			else
				prev->next = node->next;
			free_key_value_and_node(node);
			data->env_table->count--;
			return ;
		}
		prev = node;
		node = node->next;
	}
}

int	unset(char **args, t_data *data)
{
	int	i;

	if (!args)
		return (EXIT_SUCCESS);
	i = 0;
	while (args[i])
	{
		single_unset(args[i], data);
		i++;
	}
	data->env_arr = hash_table_to_arr(data);
	return (EXIT_SUCCESS);
}
