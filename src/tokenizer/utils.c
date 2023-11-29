#include "minishell.h"

int	ft_isspace(int c)
{
	// printf("c: %c\n", c);
	if (c == 32 || (c >= 9 && c <= 13))
	{
		// printf("isspace\n");
		return (1);
	}
	else
	{
		// printf("is not space\n");
		return (0);
	}
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
	// printf("isregularchar\n");
	// printf("c: %c\n", c);
	if (ft_isspace(c) || isspecialchar(c, str))
	{
		// printf("not regular char\n");
		return (FAILURE);
	}
	// printf("regular char\n");
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
