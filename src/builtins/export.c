/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:04:21 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 23:06:03 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_valid_env_var_name(const char *str);

void	single_export(const char *key, const char *value, t_data *data)
{
	t_env_var	*node;

	node = data->env_table->table[hash(key)];
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			free(node->value);
			node->value = NULL;
			node->value = ft_strdup(value);
			if (node->value == NULL)
				free_exit(data, "Error: malloc failed\n");
			break ;
		}
		node = node->next;
	}
	insert_node_ht(key, value, data);
	data->env_table->count++;
	data->env_arr = hash_table_to_arr(data);
}

typedef struct s_export
{
	int		i;
	char	*key;
	char	*value;
	char	**key_value;
	char	**tmp_key_value;
}			t_export;

void	key_value_one_null(t_export *vars)
{
	vars->tmp_key_value = malloc(sizeof(char *) * 3);
	vars->tmp_key_value[0] = ft_strdup(vars->key);
	vars->tmp_key_value[1] = ft_strdup("");
	vars->tmp_key_value[2] = NULL;
	free(vars->key_value[0]);
	vars->key_value[0] = NULL;
	free(vars->key_value);
	vars->key_value = NULL;
	vars->key_value = vars->tmp_key_value;
	vars->key = vars->tmp_key_value[0];
	vars->value = vars->tmp_key_value[1];
}

int	export(char **args, t_data *data)
{
	t_export	vars;

	vars.i = 0;
	while (args[vars.i])
	{
		vars.key_value = ft_split(args[vars.i], '=');
		if (vars.key_value == NULL)
			free_exit(data, "Error: malloc failed\n");
		vars.key = vars.key_value[0];
		if (is_valid_env_var_name(vars.key) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (vars.key_value[1] == NULL)
			key_value_one_null(&vars);
		else
			vars.value = vars.key_value[1];
		single_export(vars.key, vars.value, data);
		free_ft_split(vars.key_value);
		vars.i++;
	}
	return (EXIT_SUCCESS);
}

int	is_valid_env_var_name(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0' || !(ft_isalpha(str[i]) || str[i] == '_'))
	{
		ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	i++;
	while (str[i] != '\0')
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
		{
			ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
