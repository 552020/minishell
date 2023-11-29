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
		next_quote = ft_strchr(current + 1, '\'');
		if (!next_quote)
			return ((char *)current);
		else
			current = next_quote + 1;
	}
	return (NULL);
}

int	count_single_quotes(const char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '\'')
			count++;
		str++;
	}
	return (count);
}

char	*remove_single_quotes(char *str)
{
	char	*dest;
	char	*ret;
	char	*unpaired_quote;
	int		str_len;
	int		quotes_count;

	if (!str)
		return (NULL);
	quotes_count = count_single_quotes(str);
	str_len = ft_strlen(str) - quotes_count;
	dest = ft_calloc(str_len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	ret = dest;
	unpaired_quote = find_unpaired_quote(str);
	while (*str)
	{
		if (*str != '\'' || (str == unpaired_quote))
			*dest++ = *str;
		str++;
	}
	return (ret);
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
			free(sub);
			sub = tmp;
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