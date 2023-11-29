#include "minishell.h"

int	ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	// To do : implement flags and exit code
	// update exit status here which is the argument
	int exit_code;
	int exit_status;
	exit_code = 0;
	char *num_str;

	if (ft_strlen(node->cmd) == 4 && !node->args[0])
		exit_code = 0;
	else if (node->args[1])
	{
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
		exit_status = 1;
		return (exit_status);
	}
	else if (ft_strlen(node->cmd) == 4 && node->args[0])
	{
		exit_code = ft_atoi(node->args[0]);
		while (exit_code > 255)
			exit_code -= 256;
		while (exit_code < 0)
			exit_code += 256;
	}
	else if (node->cmd[4] == '(' && node->cmd[ft_strlen(node->cmd) - 1] == ')'
		&& !node->args)
	{
		num_str = ft_substr(node->cmd, 5, ft_strlen(node->cmd) - 6);
		// printf("num_str: %s\n", num_str);

		exit_code = ft_atoi(num_str);
		while (exit_code > 255)
			exit_code -= 256;
		while (exit_code < 0)
			exit_code += 256;
	}
	else
	{
		ft_putstr_fd("syntax error\n", STDERR_FILENO);
		exit_status = 2;
		return (exit_status);
	}

	if (node)
		free_ast(node);
	if (table)
		free_hash_table(table);
	if (envp)
		free_envp(envp);
	exit(exit_code);
}