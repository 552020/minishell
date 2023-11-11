#include "minishell.h"

void	append_first_arg(t_ast_node *node, char *arg)
{
	node->args = (char **)malloc(sizeof(char *) * 2);
	if (node->args == NULL)
	{
		perror("malloc error\n");
		free_ast(node);
		exit(FAILURE) ;
	}
	node->args[0] = ft_strdup(arg);
	if (node->args[0] == NULL)
	{
		;
	}
	node->args[1] = NULL;
}

void	append_other_args(t_ast_node *node, char *arg)
{
	char	**new_args;
	int		size;
	int		i;

	size = 0;
	while (node->args[size] != NULL)
		size++;
	new_args = (char **)malloc(sizeof(char *) * (size + 2));
	if (new_args == NULL)
	{
		free_ast(node);
		perror("malloc error\n");
		exit(FAILURE) ;
	}
	i = -1;
	while (++i < size)
	{
		new_args[i] = ft_strdup(node->args[i]);
		if (new_args[i] == NULL)
		{
			free_ast(node);
			perror("malloc error\n");
			exit(FAILURE) ;
		}
	}
	free(node->args);
	node->args = new_args;
	node->args[size] = ft_strdup(arg);
	node->args[size + 1] = NULL;
}

void	append_arg_to_command_node(t_ast_node *node, char *arg)
{
	if (node->args == NULL)
		append_first_arg(node, arg);
	else
		append_other_args(node, arg);
}

void	print_and_exit(char *str)
{
	printf("%s\n", str);
	return ;
}

void	handle_cmd_and_args(t_lexeme *lexemes, int idx, t_ast_node **node)
{
	if (lexemes[idx].type == L_COMMAND)
	{
		(*node)->cmd = ft_strdup(lexemes[idx].str);
		if ((*node)->cmd == NULL)
			print_and_exit("Error: malloc node->cmd failed");
	}
	else if (lexemes[idx].type == L_ARGUMENT)
		append_arg_to_command_node(*node, lexemes[idx].str);
}