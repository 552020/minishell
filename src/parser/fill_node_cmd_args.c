/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node_cmd_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:58:26 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/30 06:58:28 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_first_arg(t_ast_node *node, char *arg, t_data *data)
{
	node->args = (char **)malloc(sizeof(char *) * 2);
	if (node->args == NULL)
		free_exit(data, "Error: malloc failed\n");
	node->args[0] = ft_strdup(arg);
	node->args[1] = NULL;
}

void	append_args_loop(t_ast_node *node, char **new_args, t_data *data, int i)
{
	new_args[i] = ft_strdup(node->args[i]);
	if (new_args[i] == NULL)
		free_exit(data, "Error: malloc failed\n");
	if (node->args[i])
	{
		free(node->args[i]);
		node->args[i] = NULL;
	}
}

void	append_other_args(t_ast_node *node, char *arg, t_data *data)
{
	char	**new_args;
	int		size;
	int		i;

	size = 0;
	while (node->args[size] != NULL)
		size++;
	new_args = (char **)malloc(sizeof(char *) * (size + 2));
	if (new_args == NULL)
		free_exit(data, "Error: malloc failed\n");
	i = -1;
	while (++i < size)
	{
		append_args_loop(node, new_args, data, i);
	}
	free(node->args);
	node->args = NULL;
	node->args = new_args;
	node->args[size] = ft_strdup(arg);
	if (node->args[size] == NULL)
		free_exit(data, "Error: malloc failed\n");
	node->args[size + 1] = NULL;
}

void	append_arg_to_command_node(t_ast_node *node, char *arg, t_data *data)
{
	if (node->args == NULL)
		append_first_arg(node, arg, data);
	else
		append_other_args(node, arg, data);
}

void	handle_cmd_and_args(t_lexeme *lexemes, int idx, t_ast_node **node,
		t_data *data)
{
	if (lexemes[idx].type == L_COMMAND)
	{
		(*node)->cmd = ft_strdup(lexemes[idx].str);
		if ((*node)->cmd == NULL)
			free_exit(data, "Error: malloc failed\n");
	}
	else if (lexemes[idx].type == L_ARGUMENT)
		append_arg_to_command_node(*node, lexemes[idx].str, data);
}
