#include "minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int	isspecialchar(char c, const char *str)
{
	if (c == '<' || c == '>' || c == '|' || c == '$' || c == '"' || c == '\'')
	{
		if (c == '$')
		{
			if (*(str + 1) != '?' && !ft_isvalidvarname(*(str + 1)))
				return (FAILURE);
		}
		if (c == '\'' || c == '"')
		{
			if (!ft_strrchr(str + 1, c))
				return (FAILURE);
		}
		return (SUCCESS);
	}
	return (FAILURE);
}

int	isregularchar(char c, const char *str)
{
	if (ft_isspace(c) || isspecialchar(c, str))
		return (FAILURE);
	return (SUCCESS);
}

void	skip_spaces(const char **str_ptr)
{
	if (ft_isspace(**str_ptr))
	{
		while (**str_ptr && ft_isspace(**str_ptr))
			(*str_ptr)++;
	}
}

void	handle_unexpected_char(const char **str_ptr)
{
	ft_putendl_fd(UNEXPECTED_CHAR_WARNING, 2);
	(*str_ptr)++;
}

char	*remove_single_quotes(char *str)
{
	char	*result;
	char	*start;
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
	while (*str)
	{
		if (*str != '\'')
			*result++ = *str;
		str++;
	}
	return (start);
}

char	*reshuffle_single_quotes(const char *input)
{
	char	*start_input;
	char	*start;
	char	*end;
	char	*sub;
	char	*tmp;
	char	*result;

	result = ft_strdup(input);
	start_input = input;
	while (*input)
	{
		if (*input == '\'')
		{
			// this means thare is a single quote but no matching single quote
			if (isregularchar(*input, input))
				break ;
			start = input;
			end = ft_strchr(input + 1, *input);
			while (start > start_input && isregularchar(*(input - 1), input))
				start--;
			while (*end && isregularchar(*end, end))
				end++;
			tmp = ft_substr(start, 0, end - start);
			sub = remove_single_quotes(tmp);
			free(tmp);
		}
		input++;
	}
	return (result);
}

char	*reshuffle_quotes(const char *input)
{
	char *start_input;
	char *start;
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