/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 03:57:49 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 03:59:36 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	free_pattern_midfixes(t_pattern *pattern)
{
	int	i;

	i = 0;
	while (pattern->midfixes[i])
	{
		free(pattern->midfixes[i]);
		pattern->midfixes[i] = NULL;
		i++;
	}
	free(pattern->midfixes);
	pattern->midfixes = NULL;
}

void	free_pattern(t_pattern *pattern)
{
	if (pattern->pattern)
	{
		free(pattern->pattern);
		pattern->pattern = NULL;
	}
	if (pattern->prefix)
	{
		free(pattern->prefix);
		pattern->prefix = NULL;
	}
	if (pattern->suffix)
	{
		free(pattern->suffix);
		pattern->suffix = NULL;
	}
	if (pattern->midfixes)
		free_pattern_midfixes(pattern);
	if (pattern->pattern_raw)
	{
		free(pattern->pattern_raw);
		pattern->pattern_raw = NULL;
	}
}