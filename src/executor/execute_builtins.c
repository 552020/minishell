/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:44:16 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/30 04:47:39 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_is_builtin(t_ast_node *node)
{
	if (ft_strncmp(node->cmd, "env", 3) == 0 && ft_strlen(node->cmd) == 3)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "export", 6) == 0
		&& ft_strlen(node->cmd) == 6)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "pwd", 3) == 0 && ft_strlen(node->cmd) == 3)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "unset", 5) == 0
		&& ft_strlen(node->cmd) == 5)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "cd", 2) == 0 && ft_strlen(node->cmd) == 2)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "echo", 4) == 0 && ft_strlen(node->cmd) == 4)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "exit", 4) == 0)
		return (SUCCESS);
	else
		return (FAILURE);
}

// you don't need to create an array, data is already there with cmd and args
int	builtin_with_args(t_ast_node *node, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strncmp(node->cmd, "export", 6) == 0 && ft_strlen(node->cmd) == 6)
	{
		// TODO: implement ARG="arg" in the lexer
		exit_status = export(node->args, data);
		node->exit_status = exit_status;
	}
	else if (ft_strncmp(node->cmd, "unset", 5) == 0
		&& ft_strlen(node->cmd) == 5)
	{
		exit_status = unset(node->args, data);
		node->exit_status = exit_status;
	}
	return (exit_status);
}

int	builtin_without_args(t_ast_node *node, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strncmp(node->cmd, "env", 3) == 0 && ft_strlen(node->cmd) == 3)
		exit_status = env(data->env_table->table);
	else if (ft_strncmp(node->cmd, "pwd", 3) == 0 && ft_strlen(node->cmd) == 3)
		exit_status = print_working_directory();
	else if (ft_strncmp(node->cmd, "cd", 2) == 0 && ft_strlen(node->cmd) == 2)
	{
		if (node->args && node->args[1])
		{
			ft_putstr_fd("", STDERR_FILENO);
			ft_putendl_fd(" too many arguments", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else if (node->args)
			exit_status = change_directory(node->args[0]);
		else
			printf("Sorry! Cd works only with some args!\n");
	}
	else if (ft_strncmp(node->cmd, "echo", 4) == 0 && ft_strlen(node->cmd) == 4)
		exit_status = echo(node);
	else if (ft_strncmp(node->cmd, "exit", 4) == 0)
		exit_status = ft_exit(node, data->env_arr, data->env_table);
	return (exit_status);
}

int	execute_builtin(t_ast_node *node, t_data *data)
{
	int	exit_status;
	int	stdout_backup;
	int	stdin_backup;

	exit_status = 0;
	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1)
		free_exit(data, "Error: dup failed\n");
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		free_exit(data, "Error: dup failed\n");
	handle_redirections(node, data);
	if ((ft_strncmp(node->cmd, "export", 6) == 0 && ft_strlen(node->cmd) == 6)
		|| (ft_strncmp(node->cmd, "unset", 5) == 0
			&& ft_strlen(node->cmd) == 5))
		exit_status = builtin_with_args(node, data);
	else
		exit_status = builtin_without_args(node, data);
	dup2(stdout_backup, STDOUT_FILENO);
	if (stdout_backup == -1)
		free_exit(data, "Error: dup2 failed\n");
	close(stdout_backup);
	dup2(stdin_backup, STDIN_FILENO);
	if (stdin_backup == -1)
		free_exit(data, "Error: dup2 failed\n");
	close(stdin_backup);
	return (exit_status);
}
