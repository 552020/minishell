/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:40:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/29 20:20:23 by bsengeze         ###   ########.fr       */
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
	cmd_and_args_arr = malloc(sizeof(char *) * (cmd_and_args_count + 1));
	if (!cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd)
		cmd_and_args_arr[0] = ft_strdup(node->cmd);
	if (!cmd_and_args_arr[0])
		free_exit(data, "Error: malloc failed\n");
	if (node->args != NULL)
	{
		while (node->args[i] != NULL)
		{
			cmd_and_args_arr[i + 1] = ft_strdup(node->args[i]);
			if (!cmd_and_args_arr[i + 1])
				free_exit(data, "Error: malloc failed\n");
			i++;
		}
	}
	cmd_and_args_arr[i + 1] = NULL;
	return (cmd_and_args_arr);
}

/* Logic for existing scripts is missing*/
void	execute_script(t_ast_node *node, t_data *data)
{
	if (ft_strncmp(&node->cmd[0], "./", 2) == 0)
	{
		if (access(node->cmd, F_OK) == 0)
		{
			perror(" ");
			exit(126);
		}
		else
		{
			perror(" ");
			exit(127);
		}
	}
}

void	execute_cmd(t_ast_node *node, t_data *data)
{
	char	*path;
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;
	char	*dir_paths;

	dir_paths = ft_getenv(data->env_table->table, "PATH");
	path = NULL;
	if (!node->cmd && !node->args)
		return ;
	else if (node->cmd)
	{
		path = path_finder(node->cmd, dir_paths, data);
		if (!path)
			execute_script(node, data);
	}
	// When would be this the case?
	// It seems never to be the case
	else
	{
		perror(" ");
		exit(127);
	}
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = build_cmd_and_args_arr(node, cmd_and_args_count, data);
	if (!cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd && cmd_and_args_arr)
	{
		if (execve(path, cmd_and_args_arr, data->env_arr) == -1)
		{
			if (path)
			{
				free(path);
				path = NULL;
			}
			if (node->cmd[0] == '\0')
			{
				perror(" ");
				exit(0);
			}
			if (node->cmd[0] == '/')
			{
				perror(" ");
				exit(126);
			}
			perror(" ");
			exit(127);
		}
	}
	if (cmd_and_args_arr)
		free_cmd_and_args_arr(cmd_and_args_arr);
}

void	execute(t_data *data, t_ast_node *node)
{
	if (node->type == N_COMMAND && data->ast_type == UNDEFINED)
	{
		data->ast_type = SINGLE_CMD_AST;
		handle_single_cmd(node, data);
	}
	else if (node->type == N_PIPE)
	{
		data->ast_type = NOT_SINGLE_CMD_AST;
		handle_pipe(node, data);
	}
}
