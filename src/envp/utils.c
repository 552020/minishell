/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:06:35 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:19:48 by slombard         ###   ########.fr       */
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

typedef struct s_hash_table_to_arr
{
	int			i;
	int			j;
	char		**envp;
	char		*temp;
	t_env_var	*node;
}				t_hash_table_to_arr;

int	init_t_hash_table_to_arr_and_checks(t_hash_table_to_arr *vars, t_data *data)
{
	vars->i = 0;
	vars->j = 0;
	vars->envp = NULL;
	vars->temp = NULL;
	vars->node = NULL;
	if (data->env_table->count == 0)
		return (0);
	if (data->env_arr != NULL)
		free_envp(data->env_arr);
	vars->envp = (char **)malloc(sizeof(char *) * (data->env_table->count + 1));
	if (vars->envp == NULL)
		free_exit(data, "Error: malloc in hash_table_to_arr failed\n");
	data->env_arr = vars->envp;
	return (1);
}

void	assign_values_while_loop(t_hash_table_to_arr *vars, t_data *data)
{
	vars->temp = ft_strjoin(vars->node->key, "=");
	if (!vars->temp)
		free_exit(data, "Error: ft_strjoin failed\n");
	vars->envp[vars->j] = ft_strjoin(vars->temp, vars->node->value);
	if (!vars->envp[vars->j])
	{
		free(vars->temp);
		vars->temp = NULL;
		free_exit(data, "Error: ft_strjoin failed\n");
	}
	free(vars->temp);
	vars->temp = NULL;
	vars->j++;
	vars->node = vars->node->next;
}

char	**hash_table_to_arr(t_data *data)
{
	t_hash_table_to_arr	vars;

	if (!init_t_hash_table_to_arr_and_checks(&vars, data))
		return (NULL);
	while (vars.i < TABLE_SIZE)
	{
		vars.node = data->env_table->table[vars.i];
		while (vars.node != NULL)
			assign_values_while_loop(&vars, data);
		vars.i++;
	}
	vars.envp[vars.j] = NULL;
	return (vars.envp);
}
