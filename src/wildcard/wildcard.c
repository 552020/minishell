/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 20:06:58 by slombard          #+#    #+#             */
/*   Updated: 2023/12/09 00:43:42 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

/*
- First while: bring pattern->start to the beginning of the pattern
- Second while; bring pattern->start to the beginning of the pattern
- Clean the pattern from double asterisks
	pattern->pattern = reduce_consecutive_char(pattern_raw, '*');
- Reset pattern->start and pattern->end to the beginning and to the end of the pattern
	pattern->start = pattern->pattern;
- Build the prefix
	asterisk = ft_strchr(pattern->pattern, '*')
- Build the midfixes
	asterisk_reader = (char *)asterisk + 1;
*/

void	build_pattern(const char *input_asterisk, const char *input_start,
		t_pattern *pattern, t_data *data)
{
	char const	*asterisk;
	char		*asterisk_reader;
	char		*pattern_raw;
	size_t		pattern_raw_len;
	int			idx;

	printf("input_asterisk: %s\n", input_asterisk);
	pattern->prefix = NULL;
	pattern->suffix = NULL;
	pattern->midfixes = NULL;
	pattern->midfixes_nbr = 0;
	pattern->input_pattern_start = (char *)input_asterisk;
	while (pattern->input_pattern_start > input_start
		&& !ft_isspace(*(pattern->input_pattern_start - 1))
		&& *(pattern->input_pattern_start - 1) != '\0')
		pattern->input_pattern_start--;
	printf("pattern->input_pattern_start: %s\n", pattern->input_pattern_start);
	pattern->input_pattern_end = pattern->input_pattern_start;
	while (*pattern->input_pattern_end
		&& !ft_isspace(*pattern->input_pattern_end))
		pattern->input_pattern_end++;
	printf("pattern->input_pattern_end: %s\n", pattern->input_pattern_end);
	pattern_raw_len = pattern->input_pattern_end - pattern->input_pattern_start;
	pattern_raw = ft_substr(pattern->input_pattern_start, 0, pattern_raw_len);
	pattern->pattern = reduce_consecutive_char(pattern_raw, '*', data);
	pattern->start = pattern->pattern;
	pattern->end = pattern->start;
	while (*pattern->end && !ft_isspace(*pattern->end))
		pattern->end++;
	pattern->pattern_len = pattern->end - pattern->start;
	free(pattern_raw);
	asterisk = ft_strchr(pattern->pattern, '*');
	asterisk_reader = (char *)asterisk;
	pattern->prefix_len = asterisk - pattern->start;
	if (pattern->prefix_len > 0)
		pattern->prefix = ft_substr(pattern->start, 0, asterisk
				- pattern->start);
	asterisk_reader = (char *)asterisk + 1;
	while (*asterisk_reader && !ft_isspace(*asterisk_reader))
	{
		if (*asterisk_reader == '*')
			pattern->midfixes_nbr++;
		asterisk_reader++;
	}
	asterisk_reader = (char *)asterisk + 1;
	if (pattern->midfixes_nbr > 0)
	{
		pattern->midfixes = malloc(sizeof(char *) * pattern->midfixes_nbr + 1);
		if (!pattern->midfixes)
			return ;
		pattern->midfixes[pattern->midfixes_nbr] = NULL;
		asterisk_reader = (char *)asterisk + 1;
		idx = 0;
		while (*asterisk_reader && !ft_isspace(*asterisk_reader))
		{
			pattern->start = asterisk_reader;
			while (*asterisk_reader && !ft_isspace(*asterisk_reader))
			{
				if (*asterisk_reader == '*')
				{
					pattern->midfix_len = asterisk_reader - pattern->start;
					pattern->midfixes[idx] = ft_substr(pattern->start, 0,
							pattern->midfix_len);
					idx++;
					break ;
				}
				asterisk_reader++;
			}
			asterisk_reader++;
		}
	}
	asterisk_reader = ft_strrchr(pattern->pattern, '*');
	if (asterisk_reader + 1)
	{
		pattern->suffix = ft_strdup(asterisk_reader + 1);
		pattern->suffix_len = ft_strlen(pattern->suffix);
	}
	else
	{
		pattern->suffix = NULL;
		pattern->suffix_len = 0;
	}
}

char	**entry_to_char(t_entry **matching, t_data *data)
{
	char	**ret;
	int		size;
	int		i;

	printf("entry_to_char:\n");
	size = 0;
	while (matching[size]->entry)
		size++;
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
	{
		free_exit(data, "Malloc failed");
		return (NULL);
	}
	ret[size] = NULL;
	i = 0;
	while (i < size)
	{
		ret[i] = matching[i]->entry;
		i++;
	}
	return (ret);
}

char	*ft_strjoin_arr(char **arr)
{
	char	*ret;
	int		idx;
	int		len;

	idx = 0;
	len = 0;
	printf("ft_strjoin_arr:\n");
	if (!arr)
		return (ft_strdup(""));
	if (!arr[0])
		return (ft_strdup(""));
	while (arr[idx])
	{
		printf("%s\n", arr[idx]);
		idx++;
	}
	idx = 0;
	while (arr[idx])
	{
		len += ft_strlen(arr[idx]);
		idx++;
	}
	ret = malloc(sizeof(char) * (len + idx));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, arr[0], len + idx);
	idx = 1;
	while (arr[idx])
	{
		ft_strlcat(ret, " ", len + idx);
		ft_strlcat(ret, arr[idx], len + idx);
		idx++;
	}
	printf("ret: %s\n", ret);
	return (ret);
}

/*
General idea:
Reduce consecutive characters to one
Partiuculare notses:
- We use this pointer so that 'str' is always pointing to the beginning of the str. Same for this one.
	while (*src_ptr)
 */
char	*reduce_consecutive_char(const char *str, char c, t_data *data)
{
	char		*ret;
	const char	*src_ptr;
	char		*dest_ptr;

	if (str == NULL)
		return (NULL);
	ret = malloc(ft_strlen(str) + 1);
	if (!ret)
	{
		free_exit(data, "Malloc failed");
		return (NULL);
	}
	src_ptr = str;
	dest_ptr = ret;
	while (*src_ptr)
	{
		*dest_ptr = *src_ptr;
		if (*src_ptr == c)
		{
			while (*src_ptr == c)
				src_ptr++;
		}
		else
			src_ptr++;
		dest_ptr++;
	}
	*dest_ptr = '\0';
	return (ret);
}

void	init_entries(t_entries *entries_ptr)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	int				idx;
	t_entry			**entries;
	int				count;

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
	while (entries->matching[i]->entry)
	{
		end = entries->matching[i]->idx;
		while (entries->matching[i]->idx)
			end++;
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
	while (entries->matching[j]->entry)
	{
		entries->matching[j]->entry = NULL;
		entries->matching[j]->idx = NULL;
		j++;
	}
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
}

char	*get_matching_entries(t_pattern *pattern, t_data *data)
{
	t_entries	entries;
	char		*ret;
	char		**ret_arr;
	int			i;

	init_entries(&entries);
	init_matching(&entries);
	if (pattern->prefix_len > 0)
		check_prefix(&entries, pattern);
	if (pattern->midfixes_nbr > 0)
		check_midfixes(&entries, pattern);
	if (pattern->suffix_len > 0)
		check_suffix(&entries, pattern);
	ret_arr = entry_to_char(entries.matching, data);
	printf("before ret_arr:\n");
	i = 0;
	while (ret_arr[i])
	{
		printf("idx: %d\n", i);
		printf("%s\n", ret_arr[i]);
		i++;
	}
	printf("after ret_arr:\n");
	ret = ft_strjoin_arr(ret_arr);
	free_entries(&entries);
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
	free(vars->after);
	free(vars->tmp);
	free(vars->matched_files);
	vars->tmp = NULL;
	vars->tmp = vars->ret;
	free(*input);
	*input = NULL;
	*input = vars->ret;
	vars->str = *input;
}

char	*wildcard_expansion(char *input, t_data *data)
{
	t_wildcard	vars;

	vars.str = input;
	while (*vars.str != '\0')
	{
		printf("*vars.str: %c\n", *vars.str);
		if (*vars.str == '\'' || *vars.str == '\"')
			wildcard_expansion_skip_quotes(&vars);
		else if (*vars.str == '*')
			wildcard_expansion_build_expansion(&vars, &input, data);
		else
			vars.str++;
	}
	printf("input: %s\n", input);
	return (input);
}
