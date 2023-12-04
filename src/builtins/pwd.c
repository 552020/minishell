/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:07:00 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 23:07:11 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_PATH_SIZE 1024

int	print_working_directory(void)
{
	char	buffer[MAX_PATH_SIZE];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		printf("%s\n", buffer);
	}
	else
	{
		perror("Error getting current working directory");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
