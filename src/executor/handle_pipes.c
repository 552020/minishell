/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:08:30 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/08 23:39:23 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_without_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table, t_free_data *free_data)
{
	pid_t	pid;

	if (command_is_builtin(node))
	{
		execute_builtin(node, envp, env_table, free_data);
		return ;
	}
	pid = fork();
	if (pid == -1)
		error_exit(node, envp, env_table);
	if (pid == 0)
		execute_cmd(node, dir_paths, envp, free_data);
	waitpid(pid, NULL, 0);
}

void	handle_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table, t_free_data *free_data)
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
		handle_redirections(node->children[0], free_data);
		handle_pipes(node->children[0], dir_paths, envp, env_table, free_data);
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
		handle_redirections(node->children[1], free_data);
		handle_pipes(node->children[1], dir_paths, envp, env_table, free_data);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void	handle_nodes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table, t_free_data *free_data)
{

	if (node->type == N_PIPE)
		handle_pipes(node, dir_paths, envp, env_table, free_data);
	else
		handle_command_node(node, dir_paths, envp, env_table, free_data);
}

void	handle_command_node(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table, t_free_data *free_data)
{
	if (command_is_builtin(node))
		execute_builtin(node, envp, env_table, free_data);
	else
		execute_cmd(node, dir_paths, envp, free_data);
}
