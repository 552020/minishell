/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:31:25 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 21:37:45 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
	envp = NULL;
}

void	free_hash_table(t_env_table *env_table)
{
	int			i;
	t_env_var	*node;
	t_env_var	*temp;

	i = 0;
	while (i < TABLE_SIZE)
	{
		node = env_table->table[i];
		while (node != NULL)
		{
			temp = node;
			node = node->next;
			free(temp->key);
			temp->key = NULL;
			free(temp->value);
			temp->value = NULL;
			free(temp);
			temp = NULL;
		}
		i++;
	}
	free(env_table);
	env_table = NULL;
}

void	ft_free_ret_envp(char **ret, size_t i)
{
	while (i--)
	{
		free(ret[i]);
		ret[i] = NULL;
	}
	free(ret);
	ret = NULL;
}
