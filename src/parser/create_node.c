#include "minishell.h"

t_ast_node	*create_node(t_node_type type)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (new_node == NULL)
	{
		// TODO: Free all allocated memory
		exit(EXIT_FAILURE);
	}
	new_node->type = type;
	new_node->data = NULL;
	new_node->args = NULL;
	new_node->input_file = NULL;
	new_node->output_file = NULL;
	new_node->append = false;
	new_node->heredoc = false;
	new_node->heredoc_del = NULL;
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;
	return (new_node);
}

t_ast_node	*build_cmd_node(t_lexeme *lexemes, int start, int end)
{
	t_ast_node *node;
	int i;

	node = create_node(N_COMMAND);
	i = start;
	while (i <= end)
	{
		handle_cmd_and_args(lexemes, i, &node);
		handle_simple_redirects(lexemes, i, &node);
		handle_double_redirects(lexemes, i, &node);
		i++;
	}
	return (node);
}