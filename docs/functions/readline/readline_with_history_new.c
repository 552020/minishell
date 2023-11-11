#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	char	*input;

	// Loop to accept user inputs
	while ((input = readline("Shell> ")) != NULL)
	{
		// Skip adding empty lines to history
		if (strlen(input) > 0)
		{
			// Add the input to the history
			add_history(input);
		}
		// Free the input buffer
		free(input);
	}
	// Clear the history and free allocated memory
	clear_history();
	return (0);
}
