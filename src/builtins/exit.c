#include "minishell.h"

void	ft_exit(int exit_code, t_ast_node *node, char **envp,
		t_env_table *table)
{
	// To do : implement flags and exit code
	// update exit status here which is the argument
	free_ast(node);
	free_hash_table(table);
	free_envp(envp);
	exit(exit_code);
}