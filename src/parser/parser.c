#include "minishell.h"

void	parse(t_ast_node **ast_root, t_lexeme *lexemes, size_t token_count)
{
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
		printf("***Parsing***\n\n");
	*ast_root = parser(lexemes, 0, token_count - 1);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
	{
		printf("\n***Printing AST***\n\n");
		print_ast(*ast_root, 7);
		printf("\n***Printing AST NEW***\n\n");
		print_ast_new(*ast_root);
		printf("\n*** AST nodes content ***\n\n");
		debug_ast(*ast_root);
	}
}

t_ast_node	*parser(t_lexeme *lexemes, int start, int end)
{
	int i;
	t_ast_node *node;

	node = NULL;
	i = end;
	while (i >= start)
	{
		if (lexemes[i].type == L_PIPE)
		{
			node = create_node(N_PIPE);
			node->children[1] = build_cmd_node(lexemes, i + 1, end);
			end = i - 1;
			i = end;
			while (i >= start && lexemes[i].type != L_PIPE)
				i--;
			if (i > start)
				node->children[0] = parser(lexemes, start, end);
			else
				node->children[0] = build_cmd_node(lexemes, start, end);
			return (node);
		}
		i--;
	}
	node = build_cmd_node(lexemes, start, end);
	return (node);
}