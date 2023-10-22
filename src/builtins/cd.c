#include "minishell.h"

int	change_directory(const char *path)
{
	if (chdir(path) != 0)
	{
		if (errno == EACCES)
		{
			ft_putstr_fd("Permission denied: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		}
		else if (errno == ENOENT)
		{
			ft_putstr_fd("Directory not found: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("Failed to change directory: ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		}
		return (1);
	}
	return (0);
}
