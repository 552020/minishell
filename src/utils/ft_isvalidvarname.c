#include "minishell.h"

int	ft_isvalidvarname(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}
