#include "minishell.h"

void	export(t_env_table *env_table, const char *key, const char *value)
{
	t_env_var *node;

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
	insert_node_ht(env_table->table, key, value);
	env_table->count++;
	// TODO: before exiting the array should be updated
}