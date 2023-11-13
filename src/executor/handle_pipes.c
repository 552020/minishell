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

	if (node->cmd != NULL && command_is_builtin(node))
	{
		execute_builtin(node, data);
		return ;
	}
	pid = fork();
	if (pid == -1)
		free_exit(data, "Error: fork failed\n");
	if (pid == 0)
		execute_cmd(node, dir_paths, data);
	waitpid(pid, NULL, 0);
}

void	handle_pipes(t_ast_node *node, char *dir_paths, t_data *data)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	// TODO: we don't need dir_paths here aymore
	(void)dir_paths;
	if (pipe(pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	left_pid = fork();
	if (left_pid == -1)
		free_exit(data, "Error: fork failed\n");
	if (left_pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		handle_redirections(node->children[0], data);
		execute(data, node->children[0]);
		// handle_pipes(node->children[0], dir_paths, data);
		exit(EXIT_SUCCESS);
	}
	right_pid = fork();
	if (right_pid == -1)
		free_exit(data, "Error: fork failed\n");
	if (right_pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		handle_redirections(node->children[1], data);
		execute(data, node->children[1]);
		// handle_pipes(node->children[1], dir_paths, data);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

// void	handle_nodes(t_ast_node *node, char *dir_paths, char **envp,
// 		t_env_table *env_table, t_data *data)
// {
// 	if (node->type == N_PIPE)
// 		handle_pipes(node, dir_paths, data);
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
