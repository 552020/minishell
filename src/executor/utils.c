#include "minishell.h"

void	error_exit(t_ast_node *node, char **envp, t_env_table *env_table)
{
	if (node)
		free_ast(node);
	if (env_table)
		free_hash_table(env_table);
	if (envp)
		free_envp(envp);
	perror("Error");
	return ;
}

char	*path_finder(char *cmd, char *dir_paths, t_data *data)
{
	char	*path;
	char	*path_except_cmd;
	int		i;
	char	**dir_path_arr;

	dir_path_arr = ft_split(dir_paths, ':');
	if (!dir_path_arr)
		free_exit(data, "Error: dir_path_arr split failed\n");
	i = 0;
	while (dir_path_arr[i])
	{
		if (cmd[0] == '/')
		{
			path = ft_strdup(cmd);
			if (!path)
				free_exit(data, "Error: path strdup failed\n");
		}
		else
		{
			path_except_cmd = ft_strjoin(dir_path_arr[i], "/");
			if (!path_except_cmd)
				free_exit(data, "Error: path_except_cmd join failed\n");
			path = ft_strjoin(path_except_cmd, cmd);
			if (!path)
				free_exit(data, "Error: path join failed\n");
			free(path_except_cmd);
			path_except_cmd = NULL;
		}
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		path = NULL;
		i++;
	}
	free_dir_path_arr(dir_path_arr);
	return (NULL);
}

void	free_dir_path_arr(char **dir_path_arr)
{
	int i;

	i = 0;
	if (dir_path_arr)
	{
		while (dir_path_arr[i])
		{
			free(dir_path_arr[i]);
			dir_path_arr[i] = NULL;
			i++;
		}
		free(dir_path_arr);
		dir_path_arr = NULL;
	}
}