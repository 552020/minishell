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

int		is_valid_env_var_name(const char *str);

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

void	free_key_and_value(char *key, char *value)
{
	if (key)
	{
		free(key);
		key = NULL;
	}
	if (value)
	{
		free(value);
		value = NULL;
	}
}

int	export(char **args, t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	char	**key_value;

	i = 0;
	while (args[i])
	{
		key_value = ft_split(args[i], '=');
		if (key_value == NULL)
			free_exit(data, "Error: malloc failed\n");
		key = key_value[0];
		if (is_valid_env_var_name(key) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (key_value[1] == NULL)
			value = ft_strdup("");
		else
			value = key_value[1];
		single_export(key, value, data);
		free_key_and_value(key, value);
		i++;
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
