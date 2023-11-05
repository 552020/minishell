/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:08:30 by bsengeze          #+#    #+#             */
/*   Updated: 2023/10/31 00:47:46 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_without_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	pid_t	pid;

	if (command_is_builtin(node))
	{
		execute_builtin(node, envp, env_table);
		return ;
	}
	pid = fork();
	if (pid == -1)
		error_exit(node, envp, env_table);
	if (pid == 0)
		execute_cmd(node, dir_paths, envp);
	waitpid(pid, NULL, 0);
}

void	handle_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
		error_exit(node, envp, env_table);
	left_pid = fork();
	if (left_pid == -1)
		error_exit(node, envp, env_table);
	if (left_pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		handle_redirections(node->children[0]);
		handle_pipes(node->children[0], dir_paths, envp, env_table);
		exit(EXIT_SUCCESS);
	}
	right_pid = fork();
	if (right_pid == -1)
		error_exit(node, envp, env_table);
	if (right_pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		handle_redirections(node->children[1]);
		handle_pipes(node->children[1], dir_paths, envp, env_table);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void	handle_nodes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{

	if (node->type == N_PIPE)
		handle_pipes(node, dir_paths, envp, env_table);
	else
		handle_command_node(node, dir_paths, envp, env_table);
}

void	handle_command_node(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	if (command_is_builtin(node))
		execute_builtin(node, envp, env_table);
	else
		execute_cmd(node, dir_paths, envp);
}
