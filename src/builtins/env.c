#include "minishell.h"

int	env(t_env_var **table)
{
	int i;
	t_env_var *node;

	i = 0;
	while (i < TABLE_SIZE)
	{
		node = table[i];
		while (node != NULL)
		{
			printf("%s=%s\n", node->key, node->value);
			node = node->next;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}