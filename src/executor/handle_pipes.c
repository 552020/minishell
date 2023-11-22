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

void	handle_single_command(t_ast_node *node, t_data *data)
{
	pid_t	pid;
	int		termsig;
	int		status;

	if (node->cmd != NULL && command_is_builtin(node))
	{
		execute_builtin(node, data);
		return ;
	}
	disable_ctrl_c_main();
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
		// Check if the process was terminated by SIGINT
		if (termsig == SIGINT)
		{
			// Force a newline to be printed only if the process was terminated by SIGINT
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
}

void	handle_commands(t_ast_node *node, t_data *data)
{
	// pid_t	pid;
	// int		termsig;
	// int		status;
	if (node->cmd != NULL && command_is_builtin(node))
	{
		execute_builtin(node, data);
		return ;
	}
	// disable_ctrl_c_main();
	// pid = fork();
	// if (pid == -1)
	// free_exit(data, "Error: fork failed\n");
	// handle_signals_child(pid);
	// if (pid == 0)
	execute_cmd(node,data);
	// waitpid(pid, NULL, 0);
	// waitpid(pid, &status, 0);
	// if (WIFSIGNALED(status))
	// {
	// 	termsig = WTERMSIG(status);
	// 	// Check if the process was terminated by SIGINT
	// 	if (termsig == SIGINT)
	// 	{
	// 		// Force a newline to be printed only if the process was terminated by SIGINT
	// 		ft_putstr_fd("\n", STDOUT_FILENO);
	// 	}
	// }
}

void	handle_pipe(t_ast_node *node, t_data *data)
{
	int		pipe_fd[2];
	int		stdout_backup;
	pid_t	left_pid;
	pid_t	right_pid;

	/* Create Pipe */
	if (pipe(pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	stdout_backup = dup(STDOUT_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	/* Traverse or execute left child */
	handle_left_child(node->children[0], data, &left_pid, pipe_fd[0]);
	/* Traverse or execute right child */
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	handle_right_child(node->children[1], data, &right_pid, pipe_fd[0]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if ((node->children[1]->cmd != NULL)
		&& !(command_is_builtin(node->children[1])))
		waitpid(right_pid, NULL, 0);
	if ((node->children[0]->cmd != NULL)
		&& !(command_is_builtin(node->children[0])))
		waitpid(left_pid, NULL, 0);
		// TODO: probably we want the exit status of the child processes

}

void	handle_left_child(t_ast_node *node,  t_data *data, pid_t *left_pid, int pipe_fd)
{
	if (node->type == N_PIPE)
		execute(data, node);
	else if (node->cmd != NULL
		&& command_is_builtin(node))
		execute_builtin(node, data);
	else if ((node->cmd != NULL)
		&& (node->type == N_COMMAND))
	{
		*left_pid = fork();
		if (*left_pid == -1)
			free_exit(data, "Error: fork failed\n");
		handle_signals_child(*left_pid);
		if (*left_pid == 0)
		{
			close(pipe_fd);
			// dup2(pipe_fd[1], STDOUT_FILENO);
			// close(pipe_fd[1]);
			handle_redirections(node, data);
			execute(data, node);
			// handle_pipe(node->children[0], dir_paths, data);
			exit(EXIT_SUCCESS);
		}
	}
}

void	handle_right_child(t_ast_node *node, t_data *data, pid_t *right_pid, int pipe_fd)
{
	// This will never happen except bonus
	if (node->type == N_PIPE)
		execute(data, node);
	// This could happen
	else if (node->cmd != NULL
		&& command_is_builtin(node))
		execute_builtin(node, data);
	else if ((node->cmd != NULL)
		&& (node->type == N_COMMAND))
	{
		*right_pid = fork();
		if (*right_pid == -1)
			free_exit(data, "Error: fork failed\n");
		handle_signals_child(*right_pid);
		if (*right_pid == 0)
		{
			// close(pipe_fd[1]);
			dup2(pipe_fd, STDIN_FILENO);
			close(pipe_fd);
			handle_redirections(node, data);
			execute(data, node);
			// handle_pipe(node->children[1], dir_paths, data);
			exit(EXIT_SUCCESS);
		}
	}
}
