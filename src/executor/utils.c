/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 16:44:43 by slombard          #+#    #+#             */
/*   Updated: 2023/12/22 00:35:49 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_exit(t_ast_node *node, char **envp, t_env_table *env_table)
{
	if (node)
		free_ast(node);
	if (env_table)
		free_hash_table(env_table);
	if (envp)
		free_envp(envp);
	perror("Error");
	return ;
}

void	free_dir_path_arr(char **dir_path_arr)
{
	int	i;

	i = 0;
	if (dir_path_arr)
	{
		while (dir_path_arr[i])
		{
			free(dir_path_arr[i]);
			dir_path_arr[i] = NULL;
			i++;
		}
		free(dir_path_arr);
		dir_path_arr = NULL;
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
