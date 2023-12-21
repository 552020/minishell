/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:57:46 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/21 22:56:15 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_node(t_node_type type, t_data *data)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (new_node == NULL)
		free_exit(data, "Error: malloc node failed\n");
	new_node->type = type;
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->input_files = NULL;
	new_node->output_files = NULL;
	new_node->append = false;
	new_node->heredoc = false;
	new_node->heredoc_fd = -1;
	new_node->heredoc_del = NULL;
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;
	new_node->pid = -1;
	new_node->exit_status = 0;
	new_node->pipe_id = -1;
	return (new_node);
}

t_ast_node	*build_cmd_node(t_lexeme *lexemes, int start, int end, t_data *data)
{
	t_ast_node	*node;
	int			i;

	node = create_node(N_COMMAND, data);
	if (node == NULL)
		free_exit(data, "Error: malloc node failed\n");
	i = start;
	while (i <= end)
	{
		handle_cmd_and_args(lexemes, i, &node, data);
		handle_simple_redirects(lexemes, i, &node, data);
		handle_double_redirects(lexemes, i, &node, data);
		i++;
	}
	return (node);
}
