/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:40:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/22 00:34:33 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Some notes and questions:
- path will be NULL if the command is not an executable file
- it's not clear when the else case would be the case
	else
	{
		perror(" ");
		exit(127);
	}
- are we freeing path also if execve doesn't fail?
*/
typedef struct s_execute_cmd
{
	char	*path;
	char	**exec_arr;
	int		cmd_and_args_count;
	char	*dir_paths;
}			t_execute_cmd;

/*
Achtung!
cmd_and_args_arr has been renamed in exec_arr cause of norminette:
the autoformatter would put otherwise the right tabs on line split
rename is only in thi function
*/
void	init_execute_cmd_vars(t_execute_cmd *vars, t_data *data)
{
	vars->path = NULL;
	vars->exec_arr = NULL;
	vars->cmd_and_args_count = 0;
	vars->dir_paths = NULL;
	vars->dir_paths = ft_getenv(data->env_table->table, "PATH");
}

void	if_node_cmd_and_vars_exec_arr(t_ast_node *node, t_execute_cmd *vars,
		t_data *data)
{
	if (!vars->path)
	{
		ft_putstr_fd(": command not found\n", 2);
		free_data(data);
		exit(127);
	}
	if (execve(vars->path, vars->exec_arr, data->env_arr) == -1)
		handle_execve_fail(node, data, vars->path);
}

void	execute_cmd(t_ast_node *node, t_data *data)
{
	t_execute_cmd	vars;

	init_execute_cmd_vars(&vars, data);
	if (!node->cmd && !node->args)
		return ;
	else if (node->cmd)
	{
		vars.path = path_finder(node->cmd, vars.dir_paths, data);
		if (!vars.path)
			execute_script(node, data);
	}
	vars.cmd_and_args_count = count_cmd_and_args(node);
	vars.exec_arr = build_cmd_and_args_arr(node, vars.cmd_and_args_count, data);
	if (!vars.exec_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd && vars.exec_arr)
		if_node_cmd_and_vars_exec_arr(node, &vars, data);
	if (vars.exec_arr)
		free_cmd_and_args_arr(vars.exec_arr);
}

void	execute_pipe(t_data *data, t_ast_node *node)
{
	(void)node;
	if (data->ast_type == UNDEFINED)
	{
		data->pipe_fds = (int **)malloc(sizeof(int *) * 2);
		if (!data->pipe_fds)
			free_exit(data, "Error: malloc failed\n");
		data->pipe_fds[0] = (int *)malloc(sizeof(int) * 2);
		data->pipe_fds[1] = NULL;
	}
	else
	{
		data->pipe_fds = (int **)ft_realloc(data->pipe_fds, sizeof(int *)
				* (data->pipes_count + 1), sizeof(int *) * (data->pipes_count
					+ 2));
		if (!data->pipe_fds)
			free_exit(data, "Error: malloc failed\n");
		data->pipe_fds[data->pipes_count] = (int *)malloc(sizeof(int) * 2);
		data->pipe_fds[data->pipes_count + 1] = NULL;
	}
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
		execute_pipe(data, node);
		data->ast_type = NOT_SINGLE_CMD_AST;
		data->pipes_count++;
		node->pipe_id = data->pipes_count;
		handle_pipe(node, data);
	}
}
