#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	printf("Received SIGINT (CTRL+C)\n");
	// clean readline
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("Received SIGQUIT (CTRL+\\)\n");
}