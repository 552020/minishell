/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_node_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:07:57 by slombard          #+#    #+#             */
/*   Updated: 2023/12/18 18:07:59 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


void	print_node_info(t_ast_node *node)
{
	int	i;

	if (!node)
	{
		printf("Node is NULL\n");
		return ;
	}
	switch (node->type)
	{
	case N_PIPE:
		printf("Type: PIPE\n");
		break ;
	case N_COMMAND:
		printf("Type: COMMAND\n");
		break ;
	default:
		printf("Type: UNKNOWN\n");
		break ;
	}
	if (node->cmd)
		printf("cmd: %s\n", node->cmd);
	else
		printf("Cmd: NULL\n");
	if (node->args)
	{
		printf("Arguments: ");
		i = 0;
		while (node->args[i] != NULL)
		{
			printf("%s", node->args[i]);
			if (node->args[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
	}
	else
		printf("Arguments: NULL\n");
	if (node->input_files)
	{
		printf("Input Redirections: ");
		i = 0;
		while (node->input_files[i] != NULL)
		{
			printf("%s", node->input_files[i]);
			if (node->input_files[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
	}
	else
		printf("Input Redirections: NULL\n");
	if (node->output_files)
	{
		printf("Output Redirections: ");
		i = 0;
		while (node->output_files[i] != NULL)
		{
			printf("%s", node->output_files[i]);
			if (node->output_files[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
		printf("Append Mode: %s\n", node->append ? "TRUE" : "FALSE");
	}
	else
		printf("Output Redirections: NULL\n");
	if (node->heredoc)
		printf("Heredoc: TRUE\n");
	else
		printf("Heredoc: FALSE\n");
	if (node->heredoc_del)
		printf("Heredoc Delimiter: %s\n", node->heredoc_del);
	else
		printf("Heredoc Delimiter: NULL\n");
	if (node->heredoc_fd != -1)
		printf("Heredoc FD: %d\n", node->heredoc_fd);
	else
		printf("Heredoc FD: -1\n");
	if (node->children[0])
	{
		printf("Child 1:\n");
		print_node_info(node->children[0]);
	}
	else
		printf("Child 1: NULL\n");
	if (node->children[1])
	{
		printf("Child 2:\n");
		print_node_info(node->children[1]);
	}
	else
		printf("Child 2: NULL\n");
	printf("-----------\n");
}