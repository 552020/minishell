#include "minishell.h"
#include "wildcard.h"

void	build_pattern(const char *input_asterisk, const char *input_start,
		t_pattern *pattern)
{
	char const	*asterisk;
	char		*asterisk_reader;
	char		*pattern_raw;
	size_t		pattern_raw_len;
	int			idx;

	pattern->prefix = NULL;
	pattern->suffix = NULL;
	pattern->midfixes = NULL;
	pattern->midfixes_nbr = 0;
	pattern->input_pattern_start = (char *)input_asterisk;
	// Bring pattern->start to the beginning of the pattern
	while (pattern->input_pattern_start > input_start
		&& !ft_isspace(*(pattern->input_pattern_start - 1))
		&& *(pattern->input_pattern_start - 1) != '\0')
		pattern->input_pattern_start--;
	// Build the pattern (raw) with possible asterisk repetitions
	pattern->input_pattern_end = pattern->input_pattern_start;
	while (*pattern->input_pattern_end
		&& !ft_isspace(*pattern->input_pattern_end))
		pattern->input_pattern_end++;
	pattern_raw_len = pattern->input_pattern_end - pattern->input_pattern_start;
	pattern_raw = ft_substr(pattern->input_pattern_start, 0, pattern_raw_len);
	// Clean the pattern from double asterisks
	pattern->pattern = reduce_consecutive_char(pattern_raw, '*');
	// Reset pattern->start and pattern->end to the beginning and to the end of the pattern
	pattern->start = pattern->pattern;
	pattern->end = pattern->start;
	while (*pattern->end && !ft_isspace(*pattern->end))
		pattern->end++;
	pattern->pattern_len = pattern->end - pattern->start;
	// Free the raw pattern
	free(pattern_raw);
	// Build the prefix
	asterisk = ft_strchr(pattern->pattern, '*');
	asterisk_reader = (char *)asterisk;
	pattern->prefix_len = asterisk - pattern->start;
	if (pattern->prefix_len > 0)
		pattern->prefix = ft_substr(pattern->start, 0, asterisk
			- pattern->start);
	// Build the midfixes
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

char	**entry_to_char(t_entry **matching)
{
	char	**ret;
	int		size;
	int		i;

	size = 0;
	while (matching[size]->entry)
		size++;
	ret = malloc(sizeof(char *) * (size + 1));
	if (!ret)
	{
		// free memory and exit
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
	while (arr[idx])
	{
		len += ft_strlen(arr[idx]);
		idx++;
	}
	ret = malloc(sizeof(char) * (len + (idx - 1) + 1));
	if (!ret)
		return (NULL);
	idx = 0;
	while (arr[idx])
	{
		ft_strlcat(ret, arr[idx], len + (idx - 1) + 1);
		if (arr[idx + 1])
			ft_strlcat(ret, " ", len + (idx - 1) + 1);
		idx++;
	}
	return (ret);
}
// Reduce consecutive characters to one
char	*reduce_consecutive_char(const char *str, char c)
{
	char		*ret;
	const char	*src_ptr;
	char		*dest_ptr;

	if (str == NULL)
		return (NULL);
	ret = malloc(ft_strlen(str) + 1);
	if (!ret)
	{
		// free memory and exit
		return (NULL);
	}
	src_ptr = str;
	dest_ptr = ret;
	// We use this pointer so that 'str' is always pointing to the beginning of the str.
	// Same for this one.
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
		// TODO: eventually set to NULL
		// think about it
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
// ft_strstr return a pointer to the first occurence of the string
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
			// The string from the idx is not long enough to contain the suffix
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

char	*get_matching_entries(t_pattern *pattern)
{
	t_entries	entries;
	char		*ret;
	char		**ret_arr;

	// Do I really need to pass te pointer here?
	init_entries(&entries);
	init_matching(&entries);
	// Check prefix
	if (pattern->prefix_len > 0)
		check_prefix(&entries, pattern);
	// Check midfixes
	if (pattern->midfixes_nbr > 0)
		check_midfixes(&entries, pattern);
	// Check suffix
	if (pattern->suffix_len > 0)
		check_suffix(&entries, pattern);
	ret_arr = entry_to_char(entries.matching);
	ret = ft_strjoin_arr(ret_arr);
	free_entries(&entries);
	return (ret);
}

char	*wildcard_expansion(char *input)
{
	const char	*str;
	char		*matched_files;
	char		*ret;
	char		*before;
	char		*after;
	char		*tmp;
	t_pattern	pattern;
	const char	*quote;

	str = input;
	while (*str != '\0')
	{
		if (*str == '\'' || *str == '\"')
		{
			quote = ft_strchr(str + 1, *str);
			if (quote)
				str = quote + 1;
			else
				str++;
		}
		else if (*str == '*')
		{
			// Build the pattern
			build_pattern(str, input, &pattern);
			// Match the entries
			matched_files = get_matching_entries(&pattern);
			// Replace the pattern with the matched files
			before = ft_substr(input, 0, pattern.input_pattern_start - input);
			after = ft_substr(pattern.input_pattern_end, 0,
				ft_strlen(pattern.input_pattern_end));
			tmp = ft_strjoin(before, matched_files);
			ret = ft_strjoin(tmp, after);
			free(before);
			free(after);
			free(tmp);
			free(matched_files);
			tmp = NULL;
			tmp = ret;
			free(input);
			input = NULL;
			input = ret;
			// Move the pointer to the end of the pattern
			str = input;
		}
		else
			str++;
	}
	return (input);
}
