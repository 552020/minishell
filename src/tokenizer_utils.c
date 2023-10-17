#include "minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int	isspecialchar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '$' || c == '"' || c == '\'')
		return (1);
	return (0);
}

int	isregularchar(char c, const char *str)
{
	if (ft_isspace(c) || isspecialchar(c))
		return (0);
	if (c == '\'' || c == '"')
	{
		if (!ft_strrchr(str, c))
			return (1);
		return (0);
	}
	return (1);
}

void	skip_spaces(const char **str_ptr)
{
	if (ft_isspace(**str_ptr))
	{
		while (**str_ptr && ft_isspace(**str_ptr))
			(*str_ptr)++;
	}
}

void	count_word_special_char(const char **str_ptr, size_t *words)
{
	char quote;

	if (isspecialchar(**str_ptr))
	{
		(*words)++;
		if (**str_ptr == '$')
		{
			(*str_ptr)++;
			while (**str_ptr && ft_isvalidvarname(**str_ptr))
				(*str_ptr)++;
		}
		else if ((**str_ptr == '<' || **str_ptr == '>') && *(*str_ptr
				+ 1) == **str_ptr)
			(*str_ptr)++;
		else if (**str_ptr == '\'' || **str_ptr == '"')
		{
			quote = **str_ptr;
			(*str_ptr)++;
			while (**str_ptr && **str_ptr != quote)
				(*str_ptr)++;
			(*str_ptr)++;
		}
	}
}