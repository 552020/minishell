#include "minishell.h"

int	ft_isvalidvarname(char c)
{
	// Check if the character is alphanumeric or an underscore
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*trim_ending_trailing_spaces(char const *str)
{
	char	*trimmed;
	int		end;
	int		size;

	if (str == NULL)
		return (NULL);
	end = ft_strlen(str) - 1;
	// while (end >= -1 && ft_isspace(str[end]))
	while (end >= 0 && ft_isspace(str[end]))
		end--;
	if (end < 0)
		return (strdup(""));
	size = end + 1;
	trimmed = ft_substr(str, 0, size);
	return (trimmed);
}
