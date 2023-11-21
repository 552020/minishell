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

void	execute_cmd(t_ast_node *node, t_data *data)
{
	char	*path;
	char	*dir_paths;
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;

	dir_paths = ft_getenv(data->env_table->table, "PATH");
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
	int	pipe_fd[2];
	int	left_pid;
	int	right_pid;
	int	backup_stdout;
	int	termsig;
	int	left_status;
	int	right_status;

	if (node->type == N_PIPE)
	{
		if (pipe(pipe_fd) == -1)
			free_exit(data, "Error: pipe failed\n");
		backup_stdout = dup(STDOUT_FILENO);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execute(data, node->children[0]);
	}
	else if (node->type == N_COMMAND)
	{
		return ;
	}
	// TODO: what does it mean that node->cmd is NULL?
	if (node->children[0]->type == N_COMMAND && node->cmd)
	{
		if ((left_pid = fork()) == -1)
			free_exit(data, "Error: fork failed\n");
		disable_ctrl_c_main();
		handle_signals_child(left_pid);
		if (left_pid == 0)
		{
			// dup2(STDOUT_FILENO, pipe_fd[1]);
			// close(pipe_fd[1]);
			handle_redirections(node->children[0], data);
			handle_command(node->children[0], data);
		}
	}
	if (node->children[1]->type == N_COMMAND && node->cmd)
	{
		if ((right_pid = fork()) == -1)
			free_exit(data, "Error: fork failed\n");
		disable_ctrl_c_main();
		handle_signals_child(right_pid);
		if (right_pid == 0)
		{
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			dup2(backup_stdout, STDOUT_FILENO);
			close(backup_stdout);
			handle_redirections(node->children[1], data);
			handle_command(node->children[1], data);
		}
	}
	else
		return ;
	dup2(backup_stdout, STDOUT_FILENO);
	close(backup_stdout);
	if (node->children[0]->type == N_COMMAND && node->cmd)
	{
		waitpid(left_pid, &left_status, 0);
		if (WIFSIGNALED(left_status))
		{
			termsig = WTERMSIG(left_status);
			if (termsig == SIGINT)
				ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	waitpid(right_pid, &right_status, 0);
	if (WIFSIGNALED(right_status))
	{
		termsig = WTERMSIG(right_status);
		if (termsig == SIGINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
}
