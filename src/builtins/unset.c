#include "minishell.h"

void	single_unset(t_env_table *env_table, const char *key, char ***envp_ptr)
{
	unsigned int	idx;
	t_env_var		*node;
	t_env_var		*prev;

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
	*envp_ptr = convert_hash_table_to_array(env_table);
}

void	unset(t_env_table *env_table, char **args, char ***envp)
{
	int i;

	i = 0;
	while (args[i])
	{
		single_unset(env_table, args[i], envp);
		i++;
	}
}