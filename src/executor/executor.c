/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:40:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/29 17:33:40 by bsengeze         ###   ########.fr       */
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
		cmd_and_args_arr[i] = NULL;
		i++;
	}
	free(cmd_and_args_arr);
	cmd_and_args_arr = NULL;
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
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;
	char	*dir_paths;

	// To do : move thıs to path finder maybe
	dir_paths = ft_getenv(data->env_table->table, "PATH");
	path = NULL;
	if (node->cmd)
	{
		path = path_finder(node->cmd, dir_paths, data);
		if (!path)
		{
			// TODO:  add free probably
			if (ft_strncmp(&node->cmd[0], "./", 2) == 0)
			{
				if (access(node->cmd, F_OK) == 0)
				{
					// File exists but is not executable
					perror(" ");
					exit(126);
				}
				else
				{
					// File does not exist
					// printf(m"1");
					perror(" ");
					exit(127);
				}
			}
			else
			{
				// printf("else statement2\n");
				// printf("2");
				perror(" ");
				exit(127);
			}
		}
	}
	else if (!node->cmd && !node->args)
	{
		// printf("else statement1\n");
		return ;
	}
	else
	{
		// printf("else statement3\n");
		// printf("3");
		perror(" ");
		exit(127);
		return ;
	}
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = build_cmd_and_args_arr(node, cmd_and_args_count, data);
	if (!cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd && cmd_and_args_arr)
	{
		// Check it if there is leak in case of error - There is!
		if (execve(path, cmd_and_args_arr, data->env_arr) == -1)
		{
			if (path)
			{
				free(path);
				path = NULL;
			}
			perror("execve error");
			exit(127);
		}
	}
	if (cmd_and_args_arr)
		free_cmd_and_args_arr(cmd_and_args_arr);
}

// We don't need the first check anymore data->ast_type == UNDEFINED
// because we calling the execute_cmd somewhere else
void	execute(t_data *data, t_ast_node *node)
{
	if (node->type == N_COMMAND && data->ast_type == UNDEFINED)
	{
		data->ast_type = SINGLE_CMD_AST;
		handle_single_command(node, data);
	}
	else if (node->type == N_PIPE)
	{
		data->ast_type = NOT_SINGLE_CMD_AST;
		// handle_pipe returns the exit status: do we need it?
		handle_pipe(node, data);
	}
}
