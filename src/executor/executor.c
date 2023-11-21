/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:40:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/12 22:29:42 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_and_args_arr(char **cmd_and_args_arr)
{
	int	i;

	i = 0;
	while (cmd_and_args_arr[i])
	{
		free(cmd_and_args_arr[i]);
		i++;
	}
	free(cmd_and_args_arr);
}

int	count_cmd_and_args(t_ast_node *node)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (node->cmd)
		count++;
	if (node->args)
	{
		while (node->args[i])
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**build_cmd_and_args_arr(t_ast_node *node, int cmd_and_args_count,
		t_data *data)
{
	char	**cmd_and_args_arr;
	int		i;

	i = 0;
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = (char **)malloc(sizeof(char *) * (cmd_and_args_count
			+ 1));
	if (!cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd)
		cmd_and_args_arr[0] = ft_strdup(node->cmd);
	if (!cmd_and_args_arr[0])
		free_exit(data, "Error: malloc failed\n");
	// cmd_and_args_arr[0] = node->cmd;
	if (node->args != NULL)
	{
		while (node->args[i] != NULL)
		{
			cmd_and_args_arr[i + 1] = ft_strdup(node->args[i]);
			if (!cmd_and_args_arr[i + 1])
				free_exit(data, "Error: malloc failed\n");
			// cmd_and_args_arr[i + 1] = node->args[i];
			i++;
		}
	}
	cmd_and_args_arr[i + 1] = NULL;
	return (cmd_and_args_arr);
}

void	execute_cmd(t_ast_node *node, char *dir_paths, t_data *data)
{
	char	*path;
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;

	handle_redirections(node, data);
	path = NULL;
	if (node->cmd)
	{
		path = path_finder(node->cmd, dir_paths);
		if (!path)
		{
			printf("command not found\n");
			return ;
		}
	}
	else
	{
		// printf("no commands to execute\n");
		return ;
	}
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = build_cmd_and_args_arr(node, cmd_and_args_count, data);
	if (!cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd && cmd_and_args_arr)
	{
		// is this correct or not? @Stefano
		// I don't know. What do you mean? @Batu
		// When I wrote this i wasn't sure for the proper exit in case of not found executable :D
		// Now I am sure it is correct!@Stefano
		if (execve(path, cmd_and_args_arr, data->env_arr) == -1)
			perror("execve error\n");
	}
	if (cmd_and_args_arr)
		free_cmd_and_args_arr(cmd_and_args_arr);
}

void	execute(t_data *data, t_ast_node *node)
{
	char	*dir_paths;

	dir_paths = ft_getenv(data->env_table->table, "PATH");
	if (node->type == N_PIPE)
		handle_pipes(node, dir_paths, data);
	else if (node->type == N_COMMAND)
	{
		if (!node->cmd)
			return ;
		handle_commands(node, dir_paths, data);
	}
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
	handle_signals_child(left_pid);
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
	handle_signals_child(right_pid);
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
	// TODO: probably we want the exit status of the child processes
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}
