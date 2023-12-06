/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_and_args_arr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 00:51:16 by slombard          #+#    #+#             */
/*   Updated: 2023/12/07 00:51:18 by slombard         ###   ########.fr       */
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
