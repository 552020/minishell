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

int	builtin_with_args(t_ast_node *node, t_data *data)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strncmp(node->cmd, "export", 6) == 0 && ft_strlen(node->cmd) == 6)
	{
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

typedef struct s_exec_builtin
{
	int	exit_status;
	int	stdout_backup;
	int	stdin_backup;
}		t_exec_builtin;

void	init_exec_builtin(t_exec_builtin *vars, t_data *data)
{
	vars->exit_status = 0;
	vars->stdout_backup = 0;
	vars->stdin_backup = 0;
	if (vars->stdout_backup == -1)
		free_exit(data, "Error: dup failed\n");
	vars->stdin_backup = dup(STDIN_FILENO);
	if (vars->stdin_backup == -1)
		free_exit(data, "Error: dup failed\n");
}

int	execute_builtin(t_ast_node *node, t_data *data)
{
	t_exec_builtin	vars;

	init_exec_builtin(&vars, data);
	if (!handle_redirections(node, data))
		exit(EXIT_FAILURE);
	if ((ft_strncmp(node->cmd, "export", 6) == 0 && ft_strlen(node->cmd) == 6)
		|| (ft_strncmp(node->cmd, "unset", 5) == 0
			&& ft_strlen(node->cmd) == 5))
		vars.exit_status = builtin_with_args(node, data);
	else
		vars.exit_status = builtin_without_args(node, data);
	dup2(vars.stdout_backup, STDOUT_FILENO);
	if (vars.stdout_backup == -1)
		free_exit(data, "Error: dup2 failed\n");
	close(vars.stdout_backup);
	dup2(vars.stdin_backup, STDIN_FILENO);
	if (vars.stdin_backup == -1)
		free_exit(data, "Error: dup2 failed\n");
	close(vars.stdin_backup);
	return (vars.exit_status);
}
