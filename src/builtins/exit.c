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

int	ft_exit(t_ast_node *node, char **envp, t_env_table *table)
{
	int		exit_code;
	int		exit_status;
	char	*num_str;

	// To do : implement flags and exit code
	// update exit status here which is the argument
	exit_code = 0;
	if (ft_strlen(node->cmd) == 4 && !node->args)
	{
		// printf("!!!!!");
		exit_code = 0;
	}
	else if (node->args[1])
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
		exit_status = 1;
		return (exit_status);
	}
	else if (ft_strlen(node->cmd) == 4 && node->args[0])
	{
		if (!exit_input_check(node->args[0]))
		{
			ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
			exit_status = 2;
			return (exit_status);
		}
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
		if (!exit_input_check(num_str))
		{
			ft_putstr_fd(" numeric argument required\n", STDERR_FILENO);
			exit_status = 2;
			return (exit_status);
		}
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
