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

int	find_parenthesis_sibling(t_lexeme *lexemes, int start, int end)
{
	int	i;
	int	parentheses_balance;

	(void)end;
	i = start;
	parentheses_balance = 0;
	while (i >= start)
	{
		if (lexemes[i].type == L_PARENTHESIS_CLOSED)
			parentheses_balance++;
		else if (lexemes[i].type == L_PARENTHESIS_OPEN)
			parentheses_balance--;
		if (parentheses_balance == 0)
			return (i);
		i--;
	}
	return (-1);
}

t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	int			i;
	t_ast_node	*node;
	int			parenthesis_sibling;

	node = NULL;
	i = end;
	while (i >= start)
	{
		if (lexemes[i].type == L_PARENTHESIS_CLOSED)
		{
			parenthesis_sibling = find_parenthesis_sibling(lexemes, start, end);
			// This will never happen because the lexer will catch it before
			// we need to implement it though
			if (parenthesis_sibling == -1)
				free_exit(data, "Error: parentheses not balanced\n");
			else if (parenthesis_sibling == start)
			{
				node = create_node(N_PARENTHESES);
				build_parentheses_node(node, lexemes, start + 1, end - 1, data);
				return (node);
			}
			else if (parenthesis_sibling > start)
			{
				i = parenthesis_sibling - 1;
			}
		}
		if (lexemes[i].type == L_PARENTHESIS_OPEN)
		{
			// This will never happen because on parenthesis open the parser will skip to the lexeme after the parenthesis
			continue ;
		}
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
