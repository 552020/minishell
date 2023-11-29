#include "minishell.h"

char	*find_unpaired_quote(const char *str)
{
	const char	*current;
	char		*next_quote;

	if (!str)
		return (NULL);
	current = str;
	while ((current = ft_strchr(current, '\'')))
	{
		// Check for the next quote
		next_quote = ft_strchr(current + 1, '\'');
		if (!next_quote)
		{
			// If there is no next quote, current points to an unpaired quote
			return ((char *)current);
		}
		else
		{
			// If a pair is found,
			// continue searching from the position after the pair
			current = next_quote + 1;
		}
	}
	// If all quotes are paired, return NULL
	return (NULL);
}

char	*remove_single_quotes(char *str)
{
	char	*result;
	char	*start;
	char	*unpaired_quote;
	int		str_len;
	int		quotes_count;

	quotes_count = 0;
	if (!str)
		return (NULL);
	start = str;
	while (*str)
	{
		if (*str == '\'')
			quotes_count++;
		str++;
	}
	str = start;
	str_len = ft_strlen(str) - quotes_count;
	result = ft_calloc(str_len + 1, sizeof(char));
	start = result;
	if (!result)
		return (NULL);
	unpaired_quote = find_unpaired_quote(str);
	while (*str)
	{
		if (*str != '\'' || (str == unpaired_quote))
			*result++ = *str;
		str++;
	}
	return (start);
}

char	*add_single_quotes(char *str)
{
	char	*result;
	char	*start;
	int		str_len;

	str_len = ft_strlen(str);
	result = ft_calloc(str_len + 3, sizeof(char));
	start = result;
	if (!result)
		return (NULL);
	*result++ = '\'';
	while (*str)
		*result++ = *str++;
	*result++ = '\'';
	return (start);
}

char	*reshuffle_single_quotes(const char *input)
{
	const char	*start_input;
	const char	*start_sub;
	char		*end_sub;
	char		*sub;
	char		*tmp;
	char		*before;
	char		*after;
	char		*result;

	if (!input)
		return (NULL);
	result = ft_strdup(input);
	start_input = input;
	while (*input)
	{
		if (*input == '\'')
		{
			// this means thare is a single quote but no matching single quote
			if (isregularchar(*input, input))
				break ;
			start_sub = input;
			end_sub = ft_strchr(input + 1, *input);
			while (start_sub > start_input && isregularchar(*(input - 1),
					input))
				start_sub--;
			while (*end_sub && (isregularchar(*end_sub, end_sub)
					|| *end_sub == '\''))
				end_sub++;
			tmp = ft_substr(start_sub, 0, end_sub - start_sub);
			sub = remove_single_quotes(tmp);
			free(tmp);
			tmp = add_single_quotes(sub);
			free(tmp);
			before = ft_substr(result, 0, start_sub - start_input);
			after = ft_strdup(end_sub);
			free(result);
			result = ft_strjoin(before, sub);
			free(before);
			free(sub);
			tmp = ft_strjoin(result, after);
			free(result);
			free(after);
			result = tmp;
		}
		input++;
	}
	return (result);
}

char	*reshuffle_quotes(const char *input)
{
	const char *start_input;
	const char *start;
	char *end;
	char *result;

	result = ft_strdup(input);
	start_input = input;
	while (*input)
	{
		if (*input == '\'' || *input == '"')
		{
			// Problem: if there is a unpaired single quote but valid double quotes
			// we will exit the loop and return the input string
			if (isregularchar(*input, input))
			{
				break ;
			}

			start = input;
			end = ft_strchr(input + 1, *input);
			(void)end;
			while (start > start_input && isregularchar(*(input - 1), input))
				start--;

			if (ft_strchr(input + 1, *input))
			{
				input = ft_strchr(input + 1, *input);
				input++;
			}
			else
				return (ft_strdup(input));
		}
		else
			input++;
	}
	return (result);
}