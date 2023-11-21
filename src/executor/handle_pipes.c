/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:08:30 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/12 22:27:04 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_commands(t_ast_node *node, char *dir_paths, t_data *data)
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
		execute_cmd(node, dir_paths, data);
	// waitpid(pid, NULL, 0);
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

void	handle_pipes(t_ast_node *node, t_data *data)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	left_pid = fork();
	if (left_pid == -1)
		free_exit(data, "Error: fork failed\n");
	handle_signals_child(left_pid);
	if (left_pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		handle_redirections(node->children[0], data);
		execute(data, node->children[0]);
		exit(EXIT_SUCCESS);
	}
	right_pid = fork();
	if (right_pid == -1)
		free_exit(data, "Error: fork failed\n");
	handle_signals_child(right_pid);
	if (right_pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		handle_redirections(node->children[1], data);
		execute(data, node->children[1]);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}
