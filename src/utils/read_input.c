#include "minishell.h"

char	*read_input(void)
{
	char *input;
	input = readline("$> ");
	if (!input)
	{
		printf("exit\n");
		exit(0);
	}
	add_history(input);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("readline: %s\n", input);
	return (input);
}