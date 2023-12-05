/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:03:10 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 23:03:38 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parenthesis(t_ast_node *cmd)
{
	if (cmd->cmd[4] == '(' && cmd->cmd[ft_strlen(cmd->cmd) - 1] == ')')
		return (1);
	return (0);
}

int	build_exit_code(char *str)
{
	int	exit_code;

	exit_code = ft_atoi(str);
	while (exit_code > 255)
		exit_code -= 256;
	while (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

int	exit_input_check(char *str, int *exit_code, t_ast_node *node)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
		{
			ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	if (node->args[1])
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
	if (node->args[0])
		*exit_code = build_exit_code(node->args[0]);
	if (check_parenthesis(node) && !node->args)
		*exit_code = build_exit_code(str);
	return (1);
}

void	free_ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	if (node)
		free_ast(node);
	if (table)
		free_hash_table(table);
	if (envp)
		free_envp(envp);
}

int	ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	int		exit_code;
	int		return_status;
	char	*num_str;

	exit_code = 0;
	if (node->args[1])
	{
		return_status = exit_input_check(node->args[0], &exit_code, node);
		if (return_status)
			return (return_status);
	}
	else if (ft_strlen(node->cmd) == 4 && node->args[0])
	{
		return_status = exit_input_check(node->args[0], &exit_code, node);
		if (return_status)
			return (return_status);
	}
	else if (check_parenthesis(node) && !node->args)
	{
		num_str = ft_substr(node->cmd, 5, ft_strlen(node->cmd) - 6);
		return_status = exit_input_check(num_str, &exit_code, node);
		if (return_status)
		{
			free(num_str);
			return (return_status);
		}
		free(num_str);
	}
	else
	{
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
		return (2);
	}
	free_ft_exit(node, envp, table);
	exit(exit_code);
}
