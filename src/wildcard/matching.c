/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matching.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 05:04:14 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 05:24:47 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	check_prefix(t_entries *entries, t_pattern *pattern)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (entries->entries[i])
	{
		if (ft_strncmp(entries->entries[i]->entry, pattern->prefix,
				pattern->prefix_len) == 0)
		{
			entries->matching[j]->entry = entries->entries[i]->entry;
			entries->matching[j]->idx = entries->entries[i]->idx
				+ pattern->prefix_len;
			j++;
		}
		i++;
	}
}

void	check_midfix(t_entries *entries, t_pattern *pattern, char *midfix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	// printf("midfix: %s\n", midfix);
	// print entries
	// while (entries->entries[i])
	// {
	// printf("entries->entries[%d]->entry: %s\n", i,
	// 		entries->entries[i]->entry);
	// 	printf("entries->entries[%d]->idx: %s\n", i, entries->entries[i]->idx);
	// 	i++;
	// }
	// printf("finished printing entries\n");
	// i = 0;
	// print matching
	// while (entries->matching[i])
	// {
	// 	// printf("entries->matching[%d]->entry: %s\n", i,
	// 		entries->matching[i]->entry);
	// 		// printf("entries->matching[%d]->idx: %s\n", i,
	// 			// entries->matching[i]->idx);
	// 		i++;
	// }
	// printf("finished printing matching\n");
	// i = 0;
	// We want to loop on entries->matching->entry not just on entries->matching
	while (entries->matching[i]->entry)
	{
		if (ft_strnstr(entries->matching[i]->idx, midfix,
				ft_strlen(entries->matching[i]->idx)) != NULL)
		{
			entries->matching[j]->entry = entries->matching[i]->entry;
			// entries->matching[j]->idx = ft_strnstr(entries->matching[i]->idx,
			// midfix, ft_strlen(entries->entries[i]->idx))
			// + pattern->midfix_len;
			entries->matching[j]->idx = ft_strnstr(entries->matching[i]->idx,
				midfix, ft_strlen(entries->matching[i]->idx))
				+ pattern->midfix_len;
			j++;
		}
		i++;
	}
	while (entries->matching[j]->entry)
	{
		entries->matching[j]->entry = NULL;
		entries->matching[j]->idx = NULL;
		j++;
	}
}

void	check_midfixes(t_entries *entries, t_pattern *pattern)
{
	size_t	idx;

	idx = 0;
	while (idx < pattern->midfixes_nbr)
	{
		pattern->midfix_len = ft_strlen(pattern->midfixes[idx]);
		check_midfix(entries, pattern, pattern->midfixes[idx]);
		idx++;
	}
}

void	all_entries_to_matching(t_entries *entries)
{
	int	i;

	i = 0;
	while (entries->entries[i])
	{
		entries->matching[i]->entry = entries->entries[i]->entry;
		entries->matching[i]->idx = entries->entries[i]->entry;
		i++;
	}
	entries->matching[i] = NULL;
}

char	*get_matching_entries(t_pattern *pattern, t_data *data)
{
	t_entries	entries;
	char		*ret;
	char		**ret_arr;

	init_entries(&entries);
	init_matching(&entries);
	if (pattern->prefix_len != 0 || pattern->midfixes_nbr != 0
		|| pattern->suffix_len != 0)
	{
		if (pattern->prefix_len > 0)
			check_prefix(&entries, pattern);
		if (pattern->midfixes_nbr > 0)
			check_midfixes(&entries, pattern);
		if (pattern->suffix_len > 0)
			check_suffix(&entries, pattern);
	}
	else if (pattern->prefix_len == 0 && pattern->midfixes_nbr == 0
		&& pattern->suffix_len == 0)
		all_entries_to_matching(&entries);
	ret_arr = entry_to_char(entries.matching, data);
	ret = ft_strjoin_arr(ret_arr);
	free_get_matching_entries(&entries, &ret_arr, pattern);
	return (ret);
}
