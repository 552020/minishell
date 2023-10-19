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

void	handle_unexpected_char(const char **str_ptr)
{
	ft_putendl_fd(UNEXPECTED_CHAR_WARNING, 2);
	(*str_ptr)++;
}
