/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:19:43 by slombard          #+#    #+#             */
/*   Updated: 2023/12/14 14:19:44 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		printf("|\n");
	else if (node->type == N_LOG_OR)
		printf("||\n");
	else if (node->type == N_LOG_AND)
		printf("&&\n");
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
	else if (node->type == N_PARENTHESES)
		printf("()\n");
	if (node->children[0])
	{
		is_last_sibling[depth] = (node->children[1] == NULL);
		print_node(node->children[0], depth + 1, is_last_sibling);
	}
	if (node->children[1])
	{
		is_last_sibling[depth] = true;
		print_node(node->children[1], depth + 1, is_last_sibling);
	}
}

void	print_ast_new(t_ast_node *root)
{
	bool is_last_sibling[100] = {false};
	// Assuming a max depth of 100; can be dynamically allocated if needed
	print_node(root, 0, is_last_sibling);
	// (void)root;
	// (void)is_last_sibling;
	// printf("hello print_ast_new\n");
}