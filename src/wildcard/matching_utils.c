/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matching_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 05:08:40 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 05:25:31 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	init_matching(t_entries *entries)
{
	int	idx;

	idx = 0;
	entries->matching = malloc(sizeof(t_entry *) * (entries->count + 1));
	if (!entries->matching)
		return ;
	entries->matching[entries->count] = NULL;
	while (entries->entries[idx])
	{
		entries->matching[idx] = malloc(sizeof(t_entry));
		if (!entries->matching[idx])
			return ;
		entries->matching[idx]->entry = NULL;
		entries->matching[idx]->idx = NULL;
		idx++;
	}
	entries->matching[idx] = NULL;
}

void	check_suffix_internal_while(t_entries *entries, t_pattern *pattern,
		int *i, int *j)
{
	char	*end;

	end = entries->matching[*i]->idx;
	while (*end && !ft_isspace(*end) && entries->matching[*i]->entry)
	{
		end++;
		if (*end == '\0')
			break ;
	}
	if (end - pattern->suffix_len < entries->matching[*i]->idx)
		(*i)++;
	else if (ft_strncmp(end - pattern->suffix_len, pattern->suffix,
			pattern->suffix_len) == 0)
	{
		entries->matching[*j]->entry = entries->matching[*i]->entry;
		entries->matching[*j]->idx = entries->matching[*i]->idx
			+ pattern->suffix_len;
		(*j)++;
	}
	if (entries->matching[*i])
		(*i)++;
}

void	check_suffix(t_entries *entries, t_pattern *pattern)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (pattern->prefix_len == 0 && pattern->midfixes_nbr == 0)
		all_entries_to_matching(entries);
	while (entries->matching[i] && entries->matching[i]->entry)
		check_suffix_internal_while(entries, pattern, &i, &j);
	while (entries->matching[j])
	{
		entries->matching[j]->entry = NULL;
		entries->matching[j]->idx = NULL;
		j++;
	}
}

void	free_get_matching_entries(t_entries *entries, char ***ret_arr,
		t_pattern *pattern)
{
	free_entries(entries);
	free(*ret_arr);
	*ret_arr = NULL;
	free_pattern(pattern);
}
