/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 22:18:18 by slombard          #+#    #+#             */
/*   Updated: 2023/12/06 22:18:20 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct ft_realpath_vars
{
	char	**tokens;
	char	*last_slash;
	int		i;
	char	*abs_path;
	char	*cwd;
	size_t	cwd_len;
}			t_ft_realpath_vars;

// void	init_ft_realpath_vars(t_ft_realpath_vars *vars)
// {
// 	vars->tokens = NULL;
// 	vars->last_slash = NULL;
// 	vars->i = 0;
// 	vars->abs_path = NULL;
// 	vars->cwd = NULL;
// 	vars->cwd_len = 0;
// }

void	build_cwd_path(t_ft_realpath_vars *vars, t_data *data, const char *path)
{
	vars->cwd = getcwd(NULL, 0);
	if (!vars->cwd)
		free_exit(data, "Error: getcwd failed\n");
	vars->cwd_len = ft_strlen(vars->cwd) + ft_strlen(path) + 2;
	vars->abs_path = malloc(vars->cwd_len);
	if (!vars->abs_path)
		free_exit(data, "Error: malloc failed\n");
	ft_strlcpy(vars->abs_path, vars->cwd, vars->cwd_len);
	ft_strlcat(vars->abs_path, "/", vars->cwd_len);
	ft_strlcat(vars->abs_path, path, vars->cwd_len);
	free(vars->cwd);
}

void	process_each_token(t_ft_realpath_vars *vars, const char *token)
{
	if (ft_strncmp(token, "..", 2) == 0 && ft_strlen(token) == 2)
	{
		vars->last_slash = ft_strrchr(vars->abs_path, '/');
		if (vars->last_slash != vars->abs_path)
			*vars->last_slash = '\0';
		else
			ft_strlcpy(vars->abs_path, "/", vars->cwd_len);
	}
	else
	{
		if (ft_strlen(vars->abs_path) > 1)
			ft_strlcat(vars->abs_path, "/", vars->cwd_len);
		ft_strlcat(vars->abs_path, token, vars->cwd_len);
	}
}

void	process_path_tokens(t_ft_realpath_vars *vars, t_data *data,
		const char *path)
{
	vars->tokens = ft_split(path, '/');
	if (!vars->tokens)
		free_exit(data, "Error: tokens split failed\n");
	vars->i = 0;
	while (vars->tokens[vars->i] != NULL)
	{
		process_each_token(vars, vars->tokens[vars->i]);
		vars->i++;
	}
	vars->i = -1;
	while (vars->tokens[++vars->i] != NULL)
		free(vars->tokens[vars->i]);
	free(vars->tokens);
}

void	build_absolute_path(t_ft_realpath_vars *vars, char *path, t_data *data)
{
	vars->abs_path = malloc(ft_strlen(path) + 1);
	if (!vars->abs_path)
		free_exit(data, "Error: malloc failed\n");
	ft_strlcpy(vars->abs_path, path, ft_strlen(path) + 1);
}

char	*ft_realpath(const char *path, t_data *data)
{
	t_ft_realpath_vars	vars;

	vars.tokens = NULL;
	vars.last_slash = NULL;
	vars.i = 0;
	vars.abs_path = NULL;
	vars.cwd = NULL;
	vars.cwd_len = 0;
	if (!path)
		return (NULL);
	build_cwd_path(&vars, data, path);
	if (!vars.cwd)
		return (NULL);
	if (ft_strncmp(path, "./", 2) == 0)
		return (vars.abs_path);
	else if (ft_strncmp(path, "../", 3) == 0)
		process_path_tokens(&vars, data, path);
	return (vars.abs_path);
}
