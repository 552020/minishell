/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 20:06:58 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 04:02:54 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	init_entries(t_entries *entries_ptr)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	int				idx;
	t_entry			**entries;
	int				count;

	// printf("init_entries:\n");
	count = 0;
	entries = NULL;
	dir = opendir(".");
	if (!dir)
		return ;
	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(dir_entry->d_name, ".", 1) != 0
			|| ft_strncmp(dir_entry->d_name, "..", 2) != 0)
			(count)++;
	}
	closedir(dir);
	entries = malloc(sizeof(t_entry *) * (count + 1));
	if (!entries)
		return ;
	entries[count] = NULL;
	dir = opendir(".");
	if (!dir)
	{
		free(entries);
		return ;
	}
	idx = 0;
	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(dir_entry->d_name, ".", 1) != 0
			|| ft_strncmp(dir_entry->d_name, "..", 2) != 0)
		{
			entries[idx] = malloc(sizeof(t_entry));
			if (!entries[idx])
				return ;
			entries[idx]->entry = ft_strdup(dir_entry->d_name);
			entries[idx]->idx = entries[idx]->entry;
			idx++;
		}
	}
	// print entries
	// printf("printing entries\n");
	idx = 0;
	while (entries[idx])
	{
		// printf("idx: %d\n", idx);
		// printf("entries[idx]->entry: %s\n", entries[idx]->entry);
		idx++;
	}
	// printf("printing entries done\n");
	entries[idx] = NULL;
	closedir(dir);
	entries_ptr->entries = entries;
	entries_ptr->count = count;
}

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
			// free(entries->matching[j]->entry);
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
	while (entries->matching[i]->entry)
	{
		if (ft_strnstr(entries->matching[i]->idx, midfix,
				ft_strlen(entries->matching[i]->idx)) != NULL)
		{
			entries->matching[j]->entry = entries->matching[i]->entry;
			entries->matching[j]->idx = ft_strnstr(entries->matching[i]->idx,
					midfix, ft_strlen(entries->entries[i]->idx))
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

void	check_suffix(t_entries *entries, t_pattern *pattern)
{
	int		i;
	int		j;
	char	*end;

	i = 0;
	j = 0;
	// printf("check_suffix:\n");
	if (pattern->prefix_len == 0 && pattern->midfixes_nbr == 0)
		all_entries_to_matching(entries);
	// while (entries->matching[i]->entry)
	while (entries->matching[i] && entries->matching[i]->idx)
	{
		// printf("idx: %d\n", i);
		end = entries->matching[i]->idx;
		while (*end && !ft_isspace(*end))
		{
			end++;
			// printf("end: %c\n", *end);
		}
		if (end - pattern->suffix_len < entries->matching[i]->idx)
		{
			i++;
			continue ;
		}
		else if (ft_strncmp(end - pattern->suffix_len, pattern->suffix,
				pattern->suffix_len) == 0)
		{
			entries->matching[j]->entry = entries->matching[i]->entry;
			entries->matching[j]->idx = entries->matching[i]->idx
				+ pattern->suffix_len;
			j++;
		}
		i++;
	}
	while (entries->matching[j])
	{
		entries->matching[j]->entry = NULL;
		entries->matching[j]->idx = NULL;
		j++;
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

void	free_entries(t_entries *entries)
{
	int	idx;

	idx = 0;
	while (entries->entries[idx])
	{
		free(entries->entries[idx]->entry);
		free(entries->entries[idx]);
		idx++;
	}
	free(entries->entries);
	idx = 0;
	while (entries->matching[idx])
	{
		free(entries->matching[idx]);
		idx++;
	}
	free(entries->matching);
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
	free_entries(&entries);
	free(ret_arr);
	ret_arr = NULL;
	free_pattern(pattern);
	return (ret);
}

typedef struct s_wilcard
{
	const char	*str;
	char		*matched_files;
	char		*ret;
	char		*before;
	char		*after;
	char		*tmp;
	t_pattern	pattern;
	const char	*quote;
}				t_wildcard;

void	wildcard_expansion_skip_quotes(t_wildcard *vars)
{
	vars->quote = ft_strchr(vars->str + 1, *vars->str);
	if (vars->quote)
		vars->str = vars->quote + 1;
	else
		vars->str++;
}

// We do this so input points always to the beginning of the string
//*input = vars->ret;
// We do this so that we can repeat the loop from the beginning in wildcard_expansion
// vars->str = *input;

void	wildcard_expansion_build_expansion(t_wildcard *vars, char **input,
		t_data *data)
{
	build_pattern(vars->str, *input, &vars->pattern, data);
	vars->matched_files = get_matching_entries(&vars->pattern, data);
	vars->before = ft_substr(*input, 0, vars->pattern.input_pattern_start
			- *input);
	vars->after = ft_substr(vars->pattern.input_pattern_end, 0,
			ft_strlen(vars->pattern.input_pattern_end));
	vars->tmp = ft_strjoin(vars->before, vars->matched_files);
	vars->ret = ft_strjoin(vars->tmp, vars->after);
	free(vars->before);
	vars->before = NULL;
	free(vars->after);
	vars->after = NULL;
	free(vars->tmp);
	vars->tmp = NULL;
	free(vars->matched_files);
	vars->matched_files = NULL;
	vars->tmp = vars->ret;
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	*input = vars->ret;
	vars->str = *input;
}

void	init_t_wildcard(t_wildcard *vars)
{
	vars->str = NULL;
	vars->matched_files = NULL;
	vars->ret = NULL;
	vars->before = NULL;
	vars->after = NULL;
	vars->tmp = NULL;
	vars->quote = NULL;
}

char	*wildcard_expansion(char *input, t_data *data)
{
	t_wildcard	vars;

	vars.str = input;
	while (*vars.str != '\0')
	{
		if (*vars.str == '\'' || *vars.str == '\"')
			wildcard_expansion_skip_quotes(&vars);
		else if (*vars.str == '*')
			wildcard_expansion_build_expansion(&vars, &input, data);
		else
			vars.str++;
	}
	return (input);
}
