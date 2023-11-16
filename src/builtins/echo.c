#include "minishell.h"

void	echo(t_ast_node *node)
{
	int i;

	i = 0;
	if (node->args == NULL)
	{
		printf("\n");
		return ;
	}
	if (node->args[0] && ft_strncmp(node->args[0], "-n", 2) == 0)
		i++;
	while (node->args[i])
	{
		printf("%s", node->args[i]);
		if (node->args[i + 1])
			printf(" ");
		i++;
	}
	if (node->args[0] && ft_strncmp(node->args[0], "-n", 2) != 0)
		printf("\n");
}