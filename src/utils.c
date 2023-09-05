#include "minishell.h"

int	ft_isvalidvarname(char c)
{
	// Check if the character is alphanumeric or an underscore
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}