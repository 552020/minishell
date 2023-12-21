/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 18:22:39 by slombard          #+#    #+#             */
/*   Updated: 2023/12/06 18:22:41 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_cmd(t_ast_node *node, t_data *data)
{
	pid_t	pid;

	if (node->cmd != NULL && command_is_builtin(node))
	{
		node->exit_status = execute_builtin(node, data);
		return (node->exit_status);
	}
	pid = fork();
	if (pid == -1)
		free_exit(data, "Error: fork failed\n");
	if (pid != 0)
		node->pid = pid;
	handle_signals_child(pid);
	if (pid == 0)
	{
		if (handle_redirections(node, data))
			execute_cmd(node, data);
		else
		{
			perror(" ");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	return (node->exit_status);
}
