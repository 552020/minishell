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
	if (*str == 't')
		*return_status = 1;
	else
		*return_status = 2;
}

int	check_num_args(t_ast_node *node, int *exit_code, int *return_status)
{
	if (node->args[1])
	{
		write_error_and_return(" too many arguments\n", return_status);
		*exit_code = 1;
		*return_status = 1;
		return (*return_status);
	}
	else
		return (*return_status);
}

int	exit_input_check(char *str, int *exit_code, t_ast_node *node)
{
	int	i;
	int	return_status;

	return_status = 0;
	i = 0;
	return_status = check_num_args(node, exit_code, &return_status);
	while (str && str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
		{
			write_error_and_return(" numeric argument required\n",
				&return_status);
			*exit_code = 2;
			return (return_status);
		}
		i++;
	}
	if (str)
		*exit_code = build_exit_code(str);
	return (return_status);
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

	printf("exit\n");
	num_str = NULL;
	exit_code = 0;
	return_status = 0;
	if (node->args && node->args[1])
		return_status = exit_input_check(node->args[0], &exit_code, node);
	else if (node->args && ft_strlen(node->cmd) == 4 && node->args[0])
		return_status = exit_input_check(node->args[0], &exit_code, node);
	else if (check_parenthesis(node) && !node->args)
	{
		num_str = ft_substr(node->cmd, 5, ft_strlen(node->cmd) - 6);
		return_status = exit_input_check(num_str, &exit_code, node);
	}
	else if (node->args && !node->args[1])
		write_error_and_return("syntax error\n", &return_status);
	if (num_str)
		free(num_str);
	if (return_status)
		return (return_status);
	free_ft_exit(node, envp, table);
	exit(exit_code);
}
