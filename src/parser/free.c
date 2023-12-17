/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:11:49 by slombard          #+#    #+#             */
/*   Updated: 2023/12/13 15:11:51 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_cmd_node(t_ast_node *node)
{
	if (node->cmd)
	{
		free(node->cmd);
		node->cmd = NULL;
	}
	if (node->args)
		free_str_arr(node->args);
	if (node->input_files)
		free_str_arr(node->input_files);
	if (node->output_files)
		free_str_arr(node->output_files);
	if (node->heredoc_del)
	{
		free(node->heredoc_del);
		node->heredoc_del = NULL;
	}
}

void	free_ast(t_ast_node *node)
{
	if (node->type == N_COMMAND)
	{
		free_cmd_node(node);
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
