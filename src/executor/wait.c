/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 16:45:48 by slombard          #+#    #+#             */
/*   Updated: 2023/12/06 16:45:52 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_pid(t_ast_node *node, t_data *data)
{
	int	exit_status;
	int	termsig;

	exit_status = 0;
	(void)data;
	if (node->pid > 0)
	{
		waitpid(node->pid, &exit_status, 0);
		if (WIFEXITED(exit_status))
			exit_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
		{
			termsig = WTERMSIG(exit_status);
			if (termsig == SIGINT)
				ft_putstr_fd("\n", STDOUT_FILENO);
			if (termsig == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
			exit_status = termsig + 128;
		}
		node->exit_status = exit_status;
	}
	return (exit_status);
}

int	wait_single_command(t_ast_node *node, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (node->pid > 0)
		exit_status = wait_pid(node, data);
	return (exit_status);
}

int	wait_pipe(t_ast_node *node, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (node->children[1]->type == N_PIPE)
		wait_pipe(node->children[1], data);
	else if (node->children[1]->type == N_COMMAND
		&& node->children[1]->cmd != NULL)
		exit_status = wait_single_command(node->children[1], data);
	if (node->children[0]->type == N_PIPE)
		wait_pipe(node->children[0], data);
	else if (node->children[0]->type == N_COMMAND
		&& node->children[0]->cmd != NULL)
		wait_single_command(node->children[0], data);
	node->exit_status = exit_status;
	return (exit_status);
}

void	wait_ast(t_data *data, t_ast_node *node)
{
	if (node->type == N_COMMAND && data->ast_type == SINGLE_CMD_AST)
		wait_single_command(node, data);
	else if (node->type == N_PIPE)
		wait_pipe(node, data);
}
