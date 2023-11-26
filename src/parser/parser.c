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
	free_lexeme_arr(data->lexeme_arr);
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
			node = create_node(N_PIPE, data);
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

void	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_ast(t_ast_node *node)
{
	if (node == NULL)
		return ;
	if (node->type == N_COMMAND)
	{
		if (node->cmd)
		{
			free(node->cmd);
			node->cmd = NULL;
		}
		if (node->args)
			free_str_arr(node->args);
		if (node->input_file)
		{
			free(node->input_file);
			node->input_file = NULL;
		}
		if (node->output_file)
		{
			free(node->output_file);
			node->output_file = NULL;
		}
		if (node->heredoc_del)
		{
			free(node->heredoc_del);
			node->heredoc_del = NULL;
		}
	}
	else if (node->type == N_PIPE)
	{
		free_ast(node->children[0]);
		free_ast(node->children[1]);
	}
	if (node)
	{
		free(node);
		node = NULL;
	}
}