#include "minishell.h"

/* Note 1 - CTRL D and readline*/
char	*read_input(void)
{
	char	*input;

	input = readline("$> ");
	add_history(input);
	if (!input) // Handle EOF (End Of File, ctrl-D)
	{
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("readline: %s\n", input);
	return (input);
}
/* Note 1 - CTRL - D and readline
CTRL D is not a signal.
It is a special character that is read by the terminal driver when you press the key. The terminal driver interprets it as an indication that the end of the file has been reached. Basically EOF is sent to the stdin stream.
If there is no input on stdin, then the readline function will return NULL.
If there is input on stdin,
	then readline will do nothing but wait for the user to press enter. When the user presses enter,
	readline will return the input that was on stdin.
*/