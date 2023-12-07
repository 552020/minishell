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

typedef struct s_handle_pipe
{
	int		pipe_fd[2];
	int		stdout_backup;
	pid_t	l_pid;
	pid_t	r_pid;
	int		status_l;
	int		status_r;
}			t_handle_pipe;

int	handle_pipe(t_ast_node *node, t_data *data)
{
	t_handle_pipe	vars;

	vars.status_r = 0;
	if (pipe(vars.pipe_fd) == -1)
		free_exit(data, "Error: pipe failed\n");
	vars.stdout_backup = dup(STDOUT_FILENO);
	dup2(vars.pipe_fd[1], STDOUT_FILENO);
	close(vars.pipe_fd[1]);
	handle_l_child(node->children[0], data, &vars.l_pid, vars.pipe_fd[0]);
	dup2(vars.stdout_backup, STDOUT_FILENO);
	close(vars.stdout_backup);
	vars.status_r = handle_r_child(node->children[1], data, &vars.r_pid,
		vars.pipe_fd[0]);
	close(vars.pipe_fd[0]);
	return (vars.status_r);
}

void	execute_child_left(t_ast_node *node, t_data *data, int pipe_fd)
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

int	handle_l_child(t_ast_node *node, t_data *data, pid_t *l_pid, int pipe_fd)
{
	if (node->type == N_PIPE)
		execute(data, node);
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*l_pid = fork();
		if (*l_pid == -1)
			free_exit(data, "Error: fork failed\n");
		if (*l_pid != 0)
			node->pid = *l_pid;
		handle_signals_child(*l_pid);
		if (*l_pid == 0)
			execute_child_left(node, data, pipe_fd);
	}
	return (EXIT_SUCCESS);
}

void	execute_child_right(t_ast_node *node, t_data *data, int pipe_fd)
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

int	handle_r_child(t_ast_node *node, t_data *data, pid_t *r_pid, int pipe_fd)
{
	if (node->type == N_PIPE)
		execute(data, node);
	else if ((node->cmd != NULL) && (node->type == N_COMMAND))
	{
		*r_pid = fork();
		if (*r_pid == -1)
			free_exit(data, "Error: fork failed\n");
		if (*r_pid != 0)
			node->pid = *r_pid;
		handle_signals_child(*r_pid);
		if (*r_pid == 0)
			execute_child_right(node, data, pipe_fd);
	}
	return (EXIT_SUCCESS);
}
