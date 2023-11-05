#include "minishell.h"

void	error_exit(void)
{
	perror("Error");
	return ;
}

char	*path_finder(char *cmd, char *dir_paths)
{
	char *path;
	char *path_except_cmd;
	int i;
	char **dir_path_arr;

	dir_path_arr = ft_split(dir_paths, ':');
	i = 0;
	while (dir_path_arr[i])
	{
		path_except_cmd = ft_strjoin(dir_path_arr[i], "/");
		path = ft_strjoin(path_except_cmd, cmd);
		free(path_except_cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (dir_path_arr[i])
		free(dir_path_arr[i++]);
	free(dir_path_arr);
	return (NULL);
}