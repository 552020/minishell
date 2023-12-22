/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 03:13:54 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/22 03:13:58 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	if (node)
		free_ast(node);
	if (table)
		free_hash_table(table);
	if (envp)
		free_envp(envp);
}

void	free_after_execute(t_data *data)
{
	if (data->ast_root)
	{
		free_ast(data->ast_root);
		if (data->pipes_count > 0)
		{
			if (data->pipe_fds)
				free_pipe_fds(data);
			data->pipes_count = 0;
		}
		data->ast_root = NULL;
		data->ast_type = UNDEFINED;
	}
}
