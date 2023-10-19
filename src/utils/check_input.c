#include "minishell.h"

void	check_input(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		exit(1);
	}
}