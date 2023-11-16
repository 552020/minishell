#include "minishell.h"
#include "wildcard.h"
#include <assert.h>

void	test_build_pattern(const char *pattern, const char *expected_pattern)
{
	t_pattern	result;
	const char	*asterisk;

	asterisk = strchr(pattern, '*');
	if (asterisk)
	{
		build_pattern(asterisk, pattern, &result);
		// Now assert the expected values in result
		// e.g., assert(strcmp(result.prefix, "expected_prefix") == 0);
		assert(ft_strncmp(result.pattern, expected_pattern,
				result.pattern_len) == 0);
	}
	else
	{
		printf("No asterisk found in pattern: %s\n", pattern);
	}
}

int	main(int argc, char **argv)
{
	if (argc == 3)
		test_build_pattern(argv[1], argv[2]);
	else if (argc == 1)
	{
		test_build_pattern("some words before some*pattern and other after",
			"some*pattern");
		test_build_pattern("some words before some*pattern* and other after",
			"some*pattern*");
		test_build_pattern("a b c *some*pattern* d e f", "*some*pattern*");
		test_build_pattern("a b c *some*patt*ern* d e f", "*some*patt*ern*");
		test_build_pattern("a b c *some***patt*ern* d e f", "*some*patt*ern*");
		test_build_pattern("a b c ***patt*ern* d e f", "*patt*ern*");
		test_build_pattern("a b c *patt*ern*** d e f", "*patt*ern*");
		test_build_pattern("a b c *patt*ern*** d e f", "*patt*ern*");
		test_build_pattern("***patt*ern*", "*patt*ern*");
		test_build_pattern("*patt*ern***", "*patt*ern*");
		test_build_pattern("*patt*ern***", "*patt*ern*");
	}
	else
	{
		printf("Usage: %s [pattern]\n", argv[0]);
		return (1);
	}
	printf("All tests passed!\n");
	return (0);
}
