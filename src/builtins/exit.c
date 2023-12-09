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

void	write_error_and_return(char *str, int *return_status)
{
	ft_putstr_fd(str, STDERR_FILENO);
	*return_status = 2;
}

int	exit_input_check(char *str, int *exit_code, t_ast_node *node)
{
	int	i;
	int	return_status;

	return_status = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
			write_error_and_return("numeric argument requied\n",
				&return_status);
		write_error_and_return(" \n", &return_status);
		i++;
	}
	if (node->args[1])
		write_error_and_return(" too many arguments\n", &return_status);
	if (node->args[0])
		*exit_code = build_exit_code(node->args[0]);
	if (check_parenthesis(node) && !node->args)
		*exit_code = build_exit_code(str);
	return (0);
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

	num_str = NULL;
	exit_code = 0;
	if (node->args[1])
		return_status = exit_input_check(node->args[0], &exit_code, node);
	else if (ft_strlen(node->cmd) == 4 && node->args[0])
		return_status = exit_input_check(node->args[0], &exit_code, node);
	else if (check_parenthesis(node) && !node->args)
	{
		num_str = ft_substr(node->cmd, 5, ft_strlen(node->cmd) - 6);
		return_status = exit_input_check(num_str, &exit_code, node);
	}
	else
		write_error_and_return("syntax error\n", &return_status);
	if (num_str)
		free(num_str);
	if (return_status)
		return (return_status);
	free_ft_exit(node, envp, table);
	exit(exit_code);
}
