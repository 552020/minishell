#include "minishell.h"

char	*read_input(void)
{
	char *input;
	input = readline("$> ");
	add_history(input);
	if (!input) // Handle EOF (End Of File, ctrl-D)
	{
		printf("\n");
		// break:
		// TODO: what to implement instead of this break
	}
	return (input);
}