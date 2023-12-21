/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 17:44:11 by slombard          #+#    #+#             */
/*   Updated: 2023/12/18 17:44:21 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_max_depth(t_ast_node *node)
{
	int	left_depth;

	if (!node)
		return (0);
	if (node->type == N_COMMAND)
		return (1);
	left_depth = get_max_depth(node->children[0]);
	left_depth++;
	return (left_depth);
}

void	print_indentation(int depth, bool is_last_sibling[], int last_index)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		if (i == last_index)
		{
			if (is_last_sibling[i])
				printf("    ");
			else
				printf("│   ");
		}
		else
			printf("    ");
		i++;
	}
	if (depth > 0)
	{
		if (is_last_sibling[last_index])
			printf("└── ");
		else
			printf("├── ");
	}
}

void	print_node(t_ast_node *node, int depth, bool is_last_sibling[])
{
	int	i;

	if (!node)
		return ;
	print_indentation(depth, is_last_sibling, depth - 1);
	if (node->type == N_PIPE)
	{
		printf("|\n");
	}
	else if (node->type == N_COMMAND)
	{
		printf("%s", node->cmd);
		i = 0;
		while (node->args && node->args[i] != NULL)
		{
			printf(" %s", node->args[i]);
			i++;
		}
		printf("\n");
	}
	if (node->children[0])
		print_node(node->children[0], depth + 1, is_last_sibling);
	if (node->children[1])
	{
		is_last_sibling[depth] = true;
		print_node(node->children[1], depth + 1, is_last_sibling);
	}
}

void	print_ast_new(t_ast_node *root)
{
	bool is_last_sibling[100] = {false};
	print_node(root, 0, is_last_sibling);
}
