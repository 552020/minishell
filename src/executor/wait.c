#include "minishell.h"

void	wait_pid(t_ast_node *node, t_data *data)
{
	int	exit_status;
	int	termsig;

	(void)data;
	if (node->pid > 0)
	{
		waitpid(node->pid, &exit_status, 0);
		if (WIFEXITED(exit_status))
			exit_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
		{
			termsig = WTERMSIG(exit_status);
			if (termsig == SIGINT)
				ft_putstr_fd("\n", STDOUT_FILENO);
			if (termsig == SIGQUIT)
				ft_putstr_fd("Quit\n", STDOUT_FILENO);
			exit_status = termsig + 128;
		}
		node->exit_status = exit_status;
	}
}

void	wait_single_command(t_ast_node *node, t_data *data)
{
	if (node->pid > 0)
		wait_pid(node, data);
}

void	wait_pipe(t_ast_node *node, t_data *data)
{
	// Wait for left child
	if (node->children[0]->type == N_PIPE)
		wait_pipe(node->children[0], data);
	// else if (node->children[0]->type == N_COMMAND
	// 	&& node->children[0]->cmd != NULL
	// 	&& command_is_builtin(node->children[0]))
	// {
	// 	;
	// }
	else if (node->children[0]->type == N_COMMAND
		&& node->children[0]->cmd != NULL)
		// && !(command_is_builtin(node->children[0])))
		wait_single_command(node->children[0], data);
	// Wait for right child
	if (node->children[1]->type == N_PIPE)
		wait_pipe(node->children[1], data);
	// else if (node->children[1]->type == N_COMMAND
	// 	&& node->children[1]->cmd != NULL
	// 	&& command_is_builtin(node->children[1]))
	// {
	// 	;
	// }
	else if (node->children[1]->type == N_COMMAND
		&& node->children[1]->cmd != NULL)
		// && !(command_is_builtin(node->children[1])))
		wait_single_command(node->children[1], data);
}

void	wait_ast(t_data *data, t_ast_node *node)
{
	if (node->type == N_COMMAND && data->ast_type == SINGLE_CMD_AST)
		wait_single_command(node, data);
	else if (node->type == N_PIPE)
		wait_pipe(node, data);
}
