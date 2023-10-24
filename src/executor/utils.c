#include "minishell.h"

void	error_exit(void)
{
	// TODO : add free and proper exit
	perror("Error");
	exit(EXIT_FAILURE);
}