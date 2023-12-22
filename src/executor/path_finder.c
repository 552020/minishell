/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 22:40:46 by slombard          #+#    #+#             */
/*   Updated: 2023/12/06 22:40:51 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_path_finder
{
	char	*path;
	char	*path_except_cmd;
	int		i;
	char	**dir_path_arr;
}			t_path_finder;

void	init_vars_and_build_dir_path_arr(t_path_finder *vars, char *dir_paths,
		t_data *data)
{
	vars->path = NULL;
	vars->path_except_cmd = NULL;
	vars->i = 0;
	vars->dir_path_arr = ft_split(dir_paths, ':');
	if (!vars->dir_path_arr)
		free_exit(data, "Error: dir_path_arr split failed\n");
}

void	build_absolute_path_cmd(t_path_finder *vars, char *cmd, t_data *data)
{
	vars->path = ft_strdup(cmd);
	if (!vars->path)
		free_exit(data, "Error: path strdup failed\n");
}

void	build_relative_path_cmd(t_path_finder *vars, char *cmd, t_data *data)
{
	vars->path = ft_realpath(cmd, data);
	if (!vars->path)
		free_exit(data, "Error: path realpath failed\n");
}

void	build_pathless_cmd(t_path_finder *vars, char *cmd, t_data *data)
{
	vars->path_except_cmd = ft_strjoin(vars->dir_path_arr[vars->i], "/");
	if (!vars->path_except_cmd)
		free_exit(data, "Error: path_except_cmd join failed\n");
	vars->path = ft_strjoin(vars->path_except_cmd, cmd);
	if (!vars->path)
		free_exit(data, "Error: path join failed\n");
	free(vars->path_except_cmd);
	vars->path_except_cmd = NULL;
}

char	*path_finder(char *cmd, char *dir_paths, t_data *data)
{
	t_path_finder	vars;

	if (!dir_paths)
		return (NULL);
	init_vars_and_build_dir_path_arr(&vars, dir_paths, data);
	while (vars.dir_path_arr[vars.i])
	{
		if (cmd[0] == '/')
			build_absolute_path_cmd(&vars, cmd, data);
		else if ((cmd[0] == '.' && cmd[1] == '/') || (cmd[0] == '.'
				&& cmd[1] == '.' && cmd[2] == '/'))
			build_relative_path_cmd(&vars, cmd, data);
		else
			build_pathless_cmd(&vars, cmd, data);
		if (access(vars.path, X_OK) == 0)
		{
			free_dir_path_arr(vars.dir_path_arr);
			return (vars.path);
		}
		free(vars.path);
		vars.path = NULL;
		vars.i++;
	}
	free_dir_path_arr(vars.dir_path_arr);
	return (NULL);
}
