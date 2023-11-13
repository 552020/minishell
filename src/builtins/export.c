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

void	export(char **args, t_data *data)
{
	int i;
	char *key;
	char *value;
	char **key_value;

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
			free(key);
		if (value)
			free(value);
		free(key_value);
		i++;
	}

	free_cmd_and_args_arr(args);
}