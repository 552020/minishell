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

/* Notes and questions:
- Logic for existing scripts is missing
- Question: should we free before exiting?
- Now I did
*/
void	execute_script(t_ast_node *node, t_data *data)
{
	(void)data;
	if (ft_strncmp(&node->cmd[0], "./", 2) == 0)
	{
		if (access(node->cmd, F_OK) == 0)
			free_exit_code(data, " ", 126);
		else
			free_exit_code(data, " ", 127);
	}
}

void	handle_execve_fail(t_ast_node *node, t_data *data, char *path)
{
	if (path)
	{
		free(path);
		path = NULL;
	}
	if (node->cmd[0] == '\0')
		free_exit_code(data, " ", 0);
	if (node->cmd[0] == '/')
		free_exit_code(data, " ", 126);
	free_exit_code(data, " ", 127);
}
/* Some notes and questions:
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
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;
	char	*dir_paths;
}			t_execute_cmd;

void	init_execute_cmd_vars(t_execute_cmd *vars, t_data *data)
{
	vars->path = NULL;
	vars->cmd_and_args_arr = NULL;
	vars->cmd_and_args_count = 0;
	vars->dir_paths = NULL;
	vars->dir_paths = ft_getenv(data->env_table->table, "PATH");
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
	vars.cmd_and_args_arr = build_cmd_and_args_arr(node,
		vars.cmd_and_args_count, data);
	if (!vars.cmd_and_args_arr)
		free_exit(data, "Error: malloc failed\n");
	if (node->cmd && vars.cmd_and_args_arr)
	{
		if (execve(vars.path, vars.cmd_and_args_arr, data->env_arr) == -1)
			handle_execve_fail(node, data, vars.path);
	}
	if (vars.cmd_and_args_arr)
		free_cmd_and_args_arr(vars.cmd_and_args_arr);
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
