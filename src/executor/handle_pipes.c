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

int	handle_single_command(t_ast_node *node, t_data *data)
{
	pid_t	pid;
	int		termsig;
	int		status;

	if (node->cmd != NULL && command_is_builtin(node))
	{
		status = execute_builtin(node, data);
		return (status);
	}
	pid = fork();
	if (pid == -1)
		free_exit(data, "Error: fork failed\n");
	handle_signals_child(pid);
	if (pid == 0)
	{
		handle_redirections(node, data);
		execute_cmd(node, data);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		termsig = WTERMSIG(status);
		if (termsig == SIGINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		if (termsig == SIGQUIT)
			ft_putstr_fd("Quit\n", STDOUT_FILENO);
		status = termsig + 128;
	}
	return (status);
}

int	handle_pipe(t_ast_node *node, t_data *data)
{
	int		pipe_fd[2];
	int		stdout_backup;
	pid_t	left_pid;
	pid_t	right_pid;
	int		status_left;
	int		status_right;

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
	close(pipe_fd[1]);
	if ((node->children[1]->cmd != NULL)
		&& !(command_is_builtin(node->children[1])))
	{
		waitpid(right_pid, &status_right, 0);
		status_left = signal_status(status_left);
	}
	if ((node->children[0]->cmd != NULL)
		&& !(command_is_builtin(node->children[0])))
	{
		waitpid(left_pid, &status_left, 0);
		status_right = signal_status(status_right);
	}
	// TODO: probably we want the exit status of the child processes
	return (status_right);
}

int	handle_left_child(t_ast_node *node, t_data *data, pid_t *left_pid,
		int pipe_fd)
{
	int	status;

	if (node->type == N_PIPE)
		execute(data, node);
	else if (node->cmd != NULL && command_is_builtin(node))
	{
		status = execute_builtin(node, data);
		return (status);
	}
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*left_pid = fork();
		if (*left_pid == -1)
			free_exit(data, "Error: fork failed\n");
		handle_signals_child(*left_pid);
		if (*left_pid == 0)
		{
			close(pipe_fd);
			handle_redirections(node, data);
			execute_cmd(node, data);
			exit(EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}

int	handle_right_child(t_ast_node *node, t_data *data, pid_t *right_pid,
		int pipe_fd)
{
	int	status;

	if (node->type == N_PIPE)
		execute(data, node);
	else if (node->cmd != NULL && command_is_builtin(node))
	{
		status = execute_builtin(node, data);
		return (status);
	}
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*right_pid = fork();
		if (*right_pid == -1)
			free_exit(data, "Error: fork failed\n");
		handle_signals_child(*right_pid);
		if (*right_pid == 0)
		{
			dup2(pipe_fd, STDIN_FILENO);
			close(pipe_fd);
			handle_redirections(node, data);
			execute_cmd(node, data);
			exit(EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}

int	signal_status(int status)
{
	int termsig;
	int exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		return (exit_status);
	}
	else if (WIFSIGNALED(status))
	{
		termsig = WTERMSIG(status);
		if (termsig == SIGINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		return (WTERMSIG(status));
	}
	return (EXIT_SUCCESS);
}