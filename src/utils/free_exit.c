/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 17:31:05 by slombard          #+#    #+#             */
/*   Updated: 2023/12/18 17:31:07 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exit_code(t_data *data, char *error_message, int exit_code)
{
	perror(error_message);
	free_data(data);
	exit(exit_code);
}

void	free_exit(t_data *data, char *error_message)
{
	perror(error_message);
	free_data(data);
	exit(EXIT_FAILURE);
}
