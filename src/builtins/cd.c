/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:29:58 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:29:59 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
