/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:03:10 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 23:31:55 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_input_check(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	too_many_arguments(t_ast_node *node)
{
	if (!exit_input_check(node->args[0]))
	{
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		return (2);
	}
	ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
	return (1);
}

int	numeric_argument_required_and_exit_code(t_ast_node *node)
{
	int	exit_code;

	if (!exit_input_check(node->args[0]))
	{
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		return (2);
	}
	exit_code = ft_atoi(node->args[0]);
	while (exit_code > 255)
		exit_code -= 256;
	while (exit_code < 0)
		exit_code += 256;
	return (0);
}

void	free_for_exit(t_ast_node *node, t_env_table *table, char **envp)
{
	if (node)
		free_ast(node);
	if (table)
		free_hash_table(table);
	if (envp)
		free_envp(envp);
}

int	with_parenthesis(t_ast_node *node)
{
	char	*num_str;
	int		exit_code;

	num_str = ft_substr(node->cmd, 5, ft_strlen(node->cmd) - 6);
	if (!exit_input_check(num_str))
	{
		ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
		exit_code = 2;
		return (exit_code);
	}
	exit_code = ft_atoi(num_str);
	free(num_str);
	while (exit_code > 255)
		exit_code -= 256;
	while (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

int	ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strlen(node->cmd) == 4 && !node->args)
		exit_code = 0;
	else if (node->args[1])
		return (too_many_arguments(node));
	else if (ft_strlen(node->cmd) == 4 && node->args[0])
		exit_code = numeric_argument_required_and_exit_code(node);
	else if (node->cmd[4] == '(' && node->cmd[ft_strlen(node->cmd) - 1] == ')'
		&& !node->args)
		exit_code = with_parenthesis(node);
	else
	{
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
		exit_code = 2;
	}
	if (exit_code == 2)
		return (exit_code);
	free_for_exit(node, table, envp);
	exit(exit_code);
}
