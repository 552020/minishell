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

void	handle_commands(t_ast_node *node, char *dir_paths, t_data *data)
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
	execute_cmd(node, dir_paths, data);
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

void	handle_pipe(t_ast_node *node, char *dir_paths, t_data *data)
{
	int		pipe_fd[2];
	int		stdout_backup;
	pid_t	left_pid;
	pid_t	right_pid;

	// TODO: we don't need dir_paths here aymore
	(void)dir_paths;
	/* Create Pipe */
	if (pipe(pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	stdout_backup = dup(STDOUT_FILENO);
	close(pipe_fd[1]);
	/* Traverse or execute left child */
	if (node->children[0]->type == N_PIPE)
		execute(data, node->children[0]);
	else if (node->children[0]->cmd != NULL
		&& command_is_builtin(node->children[0]))
		execute_builtin(node->children[0], data);
	else if ((node->children[0]->cmd != NULL)
		&& (node->children[0]->type == N_COMMAND))
	{
		left_pid = fork();
		if (left_pid == -1)
			free_exit(data, "Error: fork failed\n");
		handle_signals_child(left_pid);
		if (left_pid == 0)
		{
			close(pipe_fd[0]);
			// dup2(pipe_fd[1], STDOUT_FILENO);
			// close(pipe_fd[1]);
			handle_redirections(node->children[0], data);
			execute(data, node->children[0]);
			// handle_pipe(node->children[0], dir_paths, data);
			exit(EXIT_SUCCESS);
		}
	}
	/* Traverse or execute right child */
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	// This will never happen
	if (node->children[1]->type == N_PIPE)
		execute(data, node->children[1]);
	// This could happen
	else if (node->children[1]->cmd != NULL
		&& command_is_builtin(node->children[1]))
		execute_builtin(node->children[1], data);
	else if ((node->children[1]->cmd != NULL)
		&& (node->children[1]->type == N_COMMAND))
	{
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
			// handle_pipe(node->children[1], dir_paths, data);
			exit(EXIT_SUCCESS);
		}
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	// TODO: probably we want the exit status of the child processes
	if ((node->children[1]->cmd != NULL)
		&& (command_is_builtin(node->children[1])))
		waitpid(right_pid, NULL, 0);
	if ((node->children[0]->cmd != NULL)
		&& (command_is_builtin(node->children[0])))
		waitpid(left_pid, NULL, 0);
}

// void	handle_nodes(t_ast_node *node, char *dir_paths, char **envp,
// 		t_env_table *env_table, t_data *data)
// {
// 	if (node->type == N_PIPE)
// 		handle_pipe(node, dir_paths, data);
// 	else
// 		handle_command_node(node, dir_paths, envp, env_table, data);
// }

// void	handle_command_node(t_ast_node *node, char *dir_paths, char **envp,
// 		t_env_table *env_table, t_data *data)
// {
// 	if (command_is_builtin(node))
// 		execute_builtin(node, data);
// 	else
// 		execute_cmd(node, dir_paths, envp, data);
// }
