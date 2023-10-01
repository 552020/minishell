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

void	append_arg_to_command_node(t_ast_node *node, char *arg)
{
	char	**new_args;
	int		size;
	int		i;

	if (node->args == NULL)
	{
		node->args = (char **)malloc(sizeof(char *) * 2);
		if (node->args == NULL)
		{
			// TODO: free memory
			exit(EXIT_FAILURE);
		}
		node->args[0] = ft_strdup(arg);
		node->args[1] = NULL;
	}
	else
	{
		size = 0;
		while (node->args[size] != NULL)
			size++;
		new_args = (char **)malloc(sizeof(char *) * (size + 2));
		if (new_args == NULL)
		{
			// free memory
			exit(EXIT_FAILURE);
		}
		i = -1;
		while (++i < size)
		{
			new_args[i] = ft_strdup(node->args[i]);
			if (new_args[i] == NULL)
			{
				// free memory
				exit(EXIT_FAILURE);
			}
		}
		free(node->args);
		node->args = new_args;
		node->args[size] = ft_strdup(arg);
		node->args[size + 1] = NULL;
	}
}
// start is the index of the first lexeme of the command
// end is the index of the last lexeme of the command, not including the pipe
t_ast_node	*build_cmd_node(t_lexeme *lexemes, int start, int end)
{
	t_ast_node	*node;
	int			i;

	node = create_node(N_COMMAND);
	i = start;
	while (i <= end)
	{
		if (lexemes[i].type == L_COMMAND)
		{
			// printf("Parsing command...\n");
			// printf("lexemes[%d].str = %s\n", i, lexemes[i].str);
			node->data = ft_strdup(lexemes[i].str);
			// printf("node->data = %s\n", node->data);
			if (node->data == NULL)
			{
				// free memory
				printf("Error: malloc node->data failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (lexemes[i].type == L_ARGUMENT)
		{
			append_arg_to_command_node(node, lexemes[i].str);
		}
		else if (lexemes[i].type == L_REDIRECT_INPUT)
			;
		else if (lexemes[i].type == L_REDIRECT_OUTPUT)
			;
		else if (lexemes[i].type == L_FILENAME_STDIN)
		{
			node->input_file = ft_strdup(lexemes[i].str);
			if (node->input_file == NULL)
			{
				// free memory
				printf("Error: malloc node->input_file failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (lexemes[i].type == L_FILENAME_STDOUT)
		{
			node->output_file = ft_strdup(lexemes[i].str);
			if (node->output_file == NULL)
			{
				// free memory
				printf("Error: malloc node->output_file failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (lexemes[i].type == L_REDIRECT_APPEND)
		{
			node->append = true;
		}
		else if (lexemes[i].type == L_HEREDOC)
		{
			node->heredoc = true;
		}
		else if (lexemes[i].type == L_HEREDOC_DELIMITER)
		{
			// printf("lexemes[%d].str = %s\n", i + 1, lexemes[i].str);
			node->heredoc_del = ft_strdup(lexemes[i].str);
		}
		else
		{
			// free memory
			printf("Error: unknown lexeme type\n");
			printf("lexemes[%d].type = %d\n", i, lexemes[i].type);
			// exit(EXIT_FAILURE);
		}
		i++;
	}
	return (node);
}

t_ast_node	*build_ast(t_lexeme *lexemes, int start, int end)
{
	int i;
	t_ast_node *node;

	node = NULL;
	i = end;
	// printf("*** build_ast ***\n");
	while (i >= start)
	{
		// printf("i = %d\n", i);
		if (lexemes[i].type == L_PIPE)
		{
			node = create_node(N_PIPE);
			node->children[1] = build_cmd_node(lexemes, i + 1, end);
			// printf("node->children[1]->type=%d\nnode->children[1]->data=%s\n",
			// 	node->children[1]->type, node->children[1]->data);
			end = i - 1;
			i = end;
			while (i >= start && lexemes[i].type != L_PIPE)
				i--;
			if (i > start)
			{
				// printf("i >= start\n");
				node->children[0] = build_ast(lexemes, start, end);
				// printf("node->children[0]->type=%d\nnode->children[0]->data=%s\n",
				// 	node->children[0]->type, node->children[0]->data);
			}
			else
			{
				// printf("i = start\n");

				node->children[0] = build_cmd_node(lexemes, start, end);
				// printf("node->children[0]->type=%d\n node->children[0]->data=%s\n",
				// 	node->children[0]->type, node->children[0]->data);
			}
			return (node);
		}
		i--;
	}
	node = build_cmd_node(lexemes, start, end);
	return (node);
}