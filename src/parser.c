#include "minishell.h"

t_ast_node	*create_node(t_node_type type, char *data)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (new_node == NULL)
	{
		// Handle memory allocation error
		return (NULL);
	}
	new_node->type = type;
	new_node->data = ft_strdup(data);
	free(data);
	new_node->input = NULL;
	new_node->output = NULL;
	new_node->children = NULL;
	return (new_node);
}
