/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:37:29 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 06:37:32 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_input(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		exit(1);
	}
}
