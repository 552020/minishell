#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>

void	test_reshuffle_single_quotes(const char *input, const char *expected)
{
	char	*result;

	result = reshuffle_single_quotes(input);
	if ((result == NULL && expected == NULL) || (result && expected
			&& strcmp(result, expected) == 0))
	{
		printf("Test passed (%s).\n", input);
	}
	else
	{
		printf("Test failed (%s). Expected: <%s>, Got: <%s>\n", input, expected,
			result);
	}
	free(result);
}

int	main(int argc, char **argv)
{
	char	*result;

	if (argc == 1)
	{
		// Test cases
		test_reshuffle_single_quotes("echo 'he''''llo'", "echo 'hello'");
		test_reshuffle_single_quotes("echo 'he'''''llo'", "echo 'hello''");
		test_reshuffle_single_quotes("echo 'he''''''llo", "echo 'he'llo'");
		test_reshuffle_single_quotes("echo he''llo", "echo 'hello'");
		test_reshuffle_single_quotes("echo 'he''llo'", "echo 'hello'");
		test_reshuffle_single_quotes("echo 'he''llo' world",
			"echo 'hello' world");
		test_reshuffle_single_quotes("echo h'ell'o world",
			"echo 'hello' world");
		test_reshuffle_single_quotes("echo 'hello'", "echo 'hello'");
		test_reshuffle_single_quotes("echo 'he'  'llo'", "echo 'he'  'llo'");
		test_reshuffle_single_quotes("echo hello", "echo hello");
		test_reshuffle_single_quotes("echo 'hello", "echo 'hello");
		test_reshuffle_single_quotes("", "");
		test_reshuffle_single_quotes(NULL, NULL);
		test_reshuffle_single_quotes("echo 'he llo' world",
			"echo 'he llo' world");
		test_reshuffle_single_quotes("'h''e''l''l''o'", "'hello'");
		test_reshuffle_single_quotes("he''llo", "'hello'");
	}
	if (argc == 2)
	{
		printf("Input: <%s>\n", argv[1]);
		result = reshuffle_single_quotes(argv[1]);
		printf("Output: <%s>\n", result);
		free(result);
	}
	if (argc == 3)
	{
		result = reshuffle_single_quotes(argv[1]);
		if (strcmp(result, argv[2]) == 0)
			printf("Test passed (%s).\n", argv[1]);
		else
			printf("Test failed (%s). Expected: <%s>, Got: <%s>\n", argv[1],
				argv[2], result);
		free(result);
	}
	return (0);
}
