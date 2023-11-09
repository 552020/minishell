#include "minishell.h"

void	single_export(t_env_table *env_table, const char *key,
		const char *value, char ***envp_ptr, t_data *data)
{
	t_env_var	*node;

	node = env_table->table[hash(key)];
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			free(node->value);
			node->value = ft_strdup(value);
			return ;
		}
		node = node->next;
	}
	insert_node_ht(env_table->table, key, value, data);
	env_table->count++;
	*envp_ptr = hash_table_to_arr(env_table, data);
}

void	export(t_env_table *env_table, char **args, char ***envp, t_data *data)
{
	int i;
	char *key;
	char *value;

	i = 0;
	while (args[i])
	{
		key = ft_split(args[i], '=')[0];
		value = ft_split(args[i], '=')[1];
		if (value == NULL)
			value = "";
		single_export(env_table, key, value, envp, data);
		i++;
	}
	free_cmd_and_args_arr(args);
}