#include "minishell.h"

void	ft_exit(int exit_code, t_ast_node *node, char **envp,
		t_env_table *table)
{
	// To do : implement flags and exit code
	// update exit status here which is the argument
	
	if(node)
		free_ast(node);
	if(table)
		free_hash_table(table);
	if(envp)
		free_envp(envp);
	exit(exit_code);
}