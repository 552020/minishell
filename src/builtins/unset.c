#include "minishell.h"

void	unset(t_env_table *env_table, const char *key)
{
	unsigned int idx;
	t_env_var *node;
	t_env_var *prev;

	idx = hash(key);
	node = env_table->table[idx];
	prev = NULL;
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			if (prev == NULL)
				env_table->table[idx] = node->next;
			else
				prev->next = node->next;
			free(node->key);
			free(node->value);
			free(node);
			env_table->count--;
			return ;
		}
		prev = node;
		node = node->next;
	}
	// TODO: before exiting the array should be updated
}