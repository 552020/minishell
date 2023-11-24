#include "minishell.h"

void	parse(t_data *data)
{
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
		printf("***Parsing***\n\n");
	data->ast_root = parser(data->lexeme_arr, 0, data->token_count - 1, data);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
	{
		printf("\n***Printing AST***\n\n");
		print_ast(data->ast_root, 7);
		printf("\n***Printing AST NEW***\n\n");
		print_ast_new(data->ast_root);
		printf("\n*** AST nodes content ***\n\n");
		debug_ast(data->ast_root);
	}
	free_lexeme_arr(data->lexeme_arr, data);
	data->lexeme_arr = NULL;
}

t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	int			i;
	t_ast_node	*node;

	node = NULL;
	i = end;
	while (i >= start)
	{
		if (lexemes[i].type == L_PIPE)
		{
			node = create_node(N_PIPE);
			node->children[1] = build_cmd_node(lexemes, i + 1, end, data);
			end = i - 1;
			i = end;
			while (i >= start && lexemes[i].type != L_PIPE)
				i--;
			if (i > start)
				node->children[0] = parser(lexemes, start, end, data);
			else
				node->children[0] = build_cmd_node(lexemes, start, end, data);
			return (node);
		}
		i--;
	}
	node = build_cmd_node(lexemes, start, end, data);
	return (node);
}
