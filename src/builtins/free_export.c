/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 08:48:53 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/22 08:48:58 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_key_and_value(char *key, char *value)
{
	if (key)
	{
		free(key);
		key = NULL;
	}
	if (value)
	{
		free(value);
		value = NULL;
	}
}

void	free_ft_split(char **key_value)
{
	int	i;

	i = 0;
	if (key_value[i])
	{
		while (key_value[i])
		{
			free(key_value[i]);
			key_value[i] = NULL;
			i++;
		}
	}
	if (key_value)
		free(key_value);
	key_value = NULL;
}
