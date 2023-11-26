#include "minishell.h"

int	echo(t_ast_node *node)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	if (node->args == NULL)
	{
		printf("\n");
		return (EXIT_SUCCESS);
	}
	while (node->args[i] && ft_strncmp(node->args[i], "-n", 2) == 0)
	{
		while (node->args[i][j] && node->args[i][j] == 'n')
			j++;
		if (node->args[i][j] == ' ' || node->args[i][j] == '\0')
			i++;
		else
			break ;
		j = 1;
	}
	while (node->args[i])
	{
		printf("%s", node->args[i]);
		if (node->args[i + 1])
			printf(" ");
		i++;
	}
	if (node->args[0] && ft_strncmp(node->args[0], "-n", 2) != 0)
		printf("\n");
	return (EXIT_SUCCESS);
}
