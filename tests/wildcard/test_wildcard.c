#include "minishell.h"
#include "wildcard.h"
#include <assert.h>

void	test_build_pattern_simple(const char *pattern,
		const char *expected_pattern)
{
	t_pattern	result;
	const char	*asterisk;

	asterisk = strchr(pattern, '*');
	if (asterisk)
	{
		build_pattern(asterisk, pattern, &result);
		// Now assert the expected values in result
		// e.g., assert(strcmp(result.prefix, "expected_prefix") == 0);
		// assert(ft_strncmp(result.pattern, expected_pattern,
		// 		result.pattern_len) == 0);
		// Manually check the condition and print values if it fails
		if (ft_strncmp(result.pattern, expected_pattern,
				result.pattern_len) != 0)
		{
			printf("Assertion failed:\n");
			printf("Expected pattern: %s\n", expected_pattern);
			printf("Result pattern: %s\n", result.pattern);
			assert(0); // Force an assertion failure
		}
	}
	else
	{
		printf("No asterisk found in pattern: %s\n", pattern);
	}
}
void	test_build_pattern(const char *str, t_pattern *expected)
{
	t_pattern	result;
	const char	*asterisk;

	asterisk = strchr(str, '*');
	if (asterisk)
	{
		build_pattern(asterisk, str, &result);
		if (ft_strncmp(result.pattern, expected->pattern,
				result.pattern_len) != 0)
		{
			printf("Assertion failed:\n");
			printf("Expected pattern: %s\n", str);
			printf("Result pattern: %s\n", result.pattern);
			assert(0);
		}
		if (ft_strncmp(result.prefix, expected->prefix, result.prefix_len) != 0)
		{
			printf("Assertion failed:\n");
			printf("Expected prefix: %s\n", expected->prefix);
			printf("Result prefix: %s\n", result.prefix);
			assert(0);
		}
		if (ft_strncmp(result.suffix, expected->suffix, result.suffix_len) != 0)
		{
			printf("Assertion failed:\n");
			printf("Expected suffix: %s\n", expected->suffix);
			printf("Result suffix: %s\n", result.suffix);
			assert(0);
		}
		if (result.midfixes_nbr != expected->midfixes_nbr)
		{
			printf("Assertion failed:\n");
			printf("Expected midfixes_nbr: %zu\n", expected->midfixes_nbr);
			printf("Result midfixes_nbr: %zu\n", result.midfixes_nbr);
			assert(0);
		}
		for (size_t i = 0; i < result.midfixes_nbr; i++)
		{
			if (ft_strncmp(result.midfixes[i], expected->midfixes[i],
					result.midfix_len) != 0)
			{
				printf("Assertion failed:\n");
				printf("Expected midfixes[%zu]: %s\n", i,
					expected->midfixes[i]);
				printf("Result midfixes[%zu]: %s\n", i, result.midfixes[i]);
				assert(0);
			}
		}
		if (result.pattern_len != expected->pattern_len)
		{
			printf("Assertion failed:\n");
			printf("Expected pattern_len: %zu\n", expected->pattern_len);
			printf("Result pattern_len: %zu\n", result.pattern_len);
			assert(0);
		}
		if (result.prefix_len != expected->prefix_len)
		{
			printf("Assertion failed:\n");
			printf("Expected prefix_len: %zu\n", expected->prefix_len);
			printf("Result prefix_len: %zu\n", result.prefix_len);
			assert(0);
		}
		if (result.suffix_len != expected->suffix_len)
		{
			printf("Assertion failed:\n");
			printf("Expected suffix_len: %zu\n", expected->suffix_len);
			printf("Result suffix_len: %zu\n", result.suffix_len);
			assert(0);
		}
	}
	else
		printf("No asterisk found in pattern: %s\n", str);
}
int	main(int argc, char **argv)
{
	if (argc == 3)
		test_build_pattern_simple(argv[1], argv[2]);
	else if (argc == 1)
	{
		t_pattern expected = {.pattern = "some*pattern",
								.prefix = "some",
								.suffix = "pattern",
								.midfixes = NULL,
								// Assuming no midfixes for this test case
								.pattern_len = strlen("some*pattern"),
								.prefix_len = strlen("some"),
								.suffix_len = strlen("pattern"),
								.midfixes_nbr = 0,
								.midfix_len = 0};
		t_pattern another = {
			.pattern = "some*other***more*complex*pattern*",
			.prefix = "some",
			.suffix = "pattern",
			.midfixes = (char *[]){"other", "more", "complex", NULL},
			.pattern_len = strlen("some*other*more*complex*pattern*"),
			.prefix_len = strlen("some"),
			.suffix_len = strlen("pattern"),
			.midfixes_nbr = 3,
		};
		test_build_pattern("some*pattern", &expected);
		test_build_pattern("some*other***more*complex*pattern*", &another);
		test_build_pattern_simple("some words before some*pattern and other after",
			"some*pattern");
		test_build_pattern_simple("some words before some*pattern* and other after",
			"some*pattern*");
		test_build_pattern_simple("a b c *some*pattern* d e f",
			"*some*pattern*");
		test_build_pattern_simple("a b c *some*patt*ern* d e f",
			"*some*patt*ern*");
		test_build_pattern_simple("a b c *some***patt*ern* d e f",
			"*some*patt*ern*");
		test_build_pattern_simple("a b c ***patt*ern* d e f", "*patt*ern*");
		test_build_pattern_simple("a b c *patt*ern*** d e f", "*patt*ern*");
		test_build_pattern_simple("a b c *patt*ern*** d e f", "*patt*ern*");
		test_build_pattern_simple("***patt*ern*", "*patt*ern*");
		test_build_pattern_simple("*patt*ern***", "*patt*ern*");
		test_build_pattern_simple("*patt*ern***", "*patt*ern*");
	}
	else
	{
		printf("Usage: %s [pattern]\n", argv[0]);
		return (1);
	}
	printf("All tests passed!\n");
	return (0);
}
