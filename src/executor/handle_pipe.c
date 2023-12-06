/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:08:30 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/12 22:27:04 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	handle_pipe(t_ast_node *node, t_data *data)
{
	int		pipe_fd[2];
	int		stdout_backup;
	pid_t	left_pid;
	pid_t	right_pid;
	int		status_left;
	int		status_right;

	status_left = 0;
	status_right = 0;
	if (pipe(pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	stdout_backup = dup(STDOUT_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	status_left = handle_left_child(node->children[0], data, &left_pid,
		pipe_fd[0]);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	status_right = handle_right_child(node->children[1], data, &right_pid,
		pipe_fd[0]);
	close(pipe_fd[0]);
	(void)status_left;
	return (status_right);
}

int	handle_left_child(t_ast_node *node, t_data *data, pid_t *left_pid,
		int pipe_fd)
{
	if (node->type == N_PIPE)
		execute(data, node);
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*left_pid = fork();
		if (*left_pid == -1)
			free_exit(data, "Error: fork failed\n");
		if (*left_pid != 0)
			node->pid = *left_pid;
		handle_signals_child(*left_pid);
		if (*left_pid == 0)
		{
			close(pipe_fd);
			if (command_is_builtin(node))
				node->exit_status = execute_builtin(node, data);
			else
			{
				if (handle_redirections(node, data))
					execute_cmd(node, data);
				else
					exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}

int	handle_right_child(t_ast_node *node, t_data *data, pid_t *right_pid,
		int pipe_fd)
{
	if (node->type == N_PIPE)
		execute(data, node);
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*right_pid = fork();
		if (*right_pid == -1)
			free_exit(data, "Error: fork failed\n");
		if (*right_pid != 0)
			node->pid = *right_pid;
		handle_signals_child(*right_pid);
		if (*right_pid == 0)
		{
			dup2(pipe_fd, STDIN_FILENO);
			close(pipe_fd);
			if (command_is_builtin(node))
				node->exit_status = execute_builtin(node, data);
			else
			{
				if (handle_redirections(node, data))
					execute_cmd(node, data);
				else
					exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}
