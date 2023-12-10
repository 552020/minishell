/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:35:16 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:05:20 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	hash(const char *key)
{
	unsigned int	hash;

	hash = -1;
	while (*key)
	{
		hash += *key++;
	}
	return (hash % TABLE_SIZE);
}

t_env_var	*create_node_ht(const char *key, const char *value, t_data *data)
{
	t_env_var	*node;

	node = (t_env_var *)malloc(sizeof(t_env_var));
	if (node == NULL)
		free_exit(data, "Error: malloc in create_node_ht failed\n");
	node->key = ft_strdup(key);
	if (node->key == NULL)
		free_exit(data, "Error: ft_strdup in create_node_ht failed\n");
	node->value = ft_strdup(value);
	if (node->value == NULL)
		free_exit(data, "Error: ft_strdup in create_node_ht failed\n");
	node->next = NULL;
	return (node);
}

void	insert_node_ht(const char *key, const char *value, t_data *data)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = data->env_table->table[idx];
	if (node == NULL)
		data->env_table->table[idx] = create_node_ht(key, value, data);
	else
	{
		if (node->key == NULL)
			printf("node->key == NULL\n");
		if (node->value == NULL)
			printf("node->value == NULL\n");
		while (node->next != NULL)
			node = node->next;
		node->next = create_node_ht(key, value, data);
	}
}
