#include "minishell.h"

void	single_export(char **args, const char *key, const char *value,
		t_data *data)
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
			{
				free_cmd_and_args_arr(args);
				free_exit(data, "Error: malloc failed\n");
			}
			return ;
		}
		node = node->next;
	}
	insert_node_ht(key, value, data);
	data->env_table->count++;
	data->env_arr = hash_table_to_arr(data);
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
		// key = ft_split(args[i], '=')[0];
		if (key_value == NULL)
		{
			free_cmd_and_args_arr(args);
			free_exit(data, "Error: malloc failed\n");
		}
		key = key_value[0];
		if (key_value[1] == NULL)
			value = ft_strdup("");
		else
			value = key_value[1];
		// value = ft_split(args[i], '=')[1];
		// if (value == NULL)
		// value = ft_strdup("");
		single_export(args, key, value, data);
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
		free(key_value);
		key_value = NULL;
		i++;
	}
	free_cmd_and_args_arr(args);
	return (EXIT_SUCCESS);
}

int	is_valid_env_var_name(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0' || !(ft_isalpha(str[i]) || str[i] == '_'))
	{
		printf("bash: %s: %s : not a valid identifier\n", "export", str);
		return (EXIT_FAILURE);
	}
	i++;
	while (str[i] != '\0')
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
		{
			printf("bash: %s: %s : not a valid identifier\n", "export", str);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
