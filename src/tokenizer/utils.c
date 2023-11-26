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
