#include "minishell.h"
#include "wildcard.h"

/* build_pattern
  This function will build a "pattern" from a string containing a wildcard. Example: from a string like "cat file*.txt",
	it will build a pattern like `file*.txt`.
  The arguments are:

	- asterisk: a pointer to the first asterisk in the string. Example: `file*.txt`
	-> `*`

	- input_start: a pointer to the beginning of the string. Example: `cat file*.txt`
	-> `c`

	- pattern_ptr: a pointer to a t_pattern variable. This variable will be filled with the pattern. We use this struct not only in this function but also in the `get_matching_entries` function.
  The return value is void but actually the point of the function is to fill the t_pattern variable,
	the pattern field, which is the whole pattern like "file*.txt". The suffix,
	prefix and midfixes fields are also filled. The prefix in this example is "file" and the suffix is ".txt". The midfixes are the strings between the asterisks. In this example,
	there are no midfixes. Examples of midfixes in a pattern like "file*hello*world*.txt*",
	"hello" and "world". The reason to distinguish between them is that we can check prefix and suffix with ft_strncmp,
	but we need to check midfixes with ft_strstr.
*/

void	build_pattern(const char *asterisk, const char *input_start,
		t_pattern *pattern_ptr)
{
	t_pattern	pattern;
	char		*asterisk_reader;
	int			idx;

	pattern = *pattern_ptr;
	pattern.prefix = NULL;
	pattern.suffix = NULL;
	pattern.midfixes = NULL;
	asterisk_reader = (char *)asterisk;
	pattern.start = asterisk;
	// Build the prefix
	while (pattern.start > input_start && !ft_isspace(*(pattern.start - 1))
		&& *(pattern.start - 1) != '\0')
		pattern.start--;
	pattern.prefix_len = asterisk - pattern.start;
	if (pattern.prefix_len > 0)
		pattern.prefix = ft_substr(pattern.start, 0, asterisk - pattern.start);
	// Build the pattern
	pattern.end = pattern.start;
	while (*pattern.end && !ft_isspace(*pattern.end))
		pattern.end++;
	pattern.pattern_len = pattern.end - pattern.start;
	pattern.pattern = ft_substr(pattern.start, 0, pattern.pattern_len);
	// Build the midfixes
	asterisk_reader = (char *)asterisk + 1;
	while (*asterisk_reader && !ft_isspace(*asterisk_reader))
	{
		if (*asterisk_reader == '*')
			pattern.midfixes_nbr++;
		asterisk_reader++;
	}
	if (pattern.midfixes_nbr > 0)
	{
		pattern.midfixes = malloc(sizeof(char *) * pattern.midfixes_nbr + 1);
		if (!pattern.midfixes)
			return ;
		pattern.midfixes[pattern.midfixes_nbr] = NULL;
		asterisk_reader = (char *)asterisk + 1;
		idx = 0;
		while (*asterisk_reader && !ft_isspace(*asterisk_reader))
		{
			pattern.start = asterisk_reader;
			while (*asterisk_reader && !ft_isspace(*asterisk_reader))
			{
				if (*asterisk_reader == '*')
				{
					pattern.midfix_len = asterisk_reader - pattern.start - 1;
					pattern.midfixes[idx] = ft_substr(pattern.start, 0,
						pattern.midfix_len);
					idx++;
					break ;
				}
				asterisk_reader++;
			}
			asterisk_reader++;
		}
	}
	// Build the suffix
	if (pattern.start != asterisk_reader)
	{
		pattern.suffix_len = asterisk_reader - pattern.start;
		pattern.suffix = ft_substr(pattern.start, 0, pattern.suffix_len);
	}
	else
		pattern.suffix_len = 0;
}

char	**entry_arr_to_char_arr(t_entry **entries, int count)
{
	char	**ret;
	int		idx;

	ret = malloc(sizeof(char *) * (count + 1));
	if (!ret)
		return (NULL);
	ret[count] = NULL;
	idx = 0;
	while (idx < count)
	{
		ret[idx] = ft_strdup(entries[idx]->entry);
		idx++;
	}
	return (ret);
}

char	*ft_strjoin_arr(char **arr, char *sep)
{
	char	*ret;
	int		idx;
	int		len;
	int		sep_len;

	idx = 0;
	len = 0;
	sep_len = ft_strlen(sep);
	while (arr[idx])
	{
		len += ft_strlen(arr[idx]);
		idx++;
	}
	ret = malloc(sizeof(char) * (len + sep_len * (idx - 1) + 1));
	if (!ret)
		return (NULL);
	idx = 0;
	while (arr[idx])
	{
		ft_strlcat(ret, arr[idx], len + sep_len * (idx - 1) + 1);
		if (arr[idx + 1])
			ft_strlcat(ret, sep, len + sep_len * (idx - 1) + 1);
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

t_entry	**build_entries_array(t_entry **entries, int *count)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	int				idx;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(dir_entry->d_name, ".", 1) != 0
			|| ft_strncmp(dir_entry->d_name, "..", 2) != 0)
			(*count)++;
	}
	closedir(dir);
	entries = malloc(sizeof(t_entry *) * (*count + 1));
	if (!entries)
		return (NULL);
	entries[*count] = NULL;
	dir = opendir(".");
	if (!dir)
	{
		free(entries);
		return (NULL);
	}
	idx = 0;
	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(dir_entry->d_name, ".", 1) != 0
			|| ft_strncmp(dir_entry->d_name, "..", 2) != 0)
		{
			entries[idx] = malloc(sizeof(t_entry));
			if (!entries[idx])
				return (NULL);
			entries[idx]->entry = ft_strdup(dir_entry->d_name);
			entries[idx]->idx = entries[idx]->entry;
			idx++;
		}
	}
	entries[idx] = NULL;
	closedir(dir);
	return (entries);
}

/* This will take an t_pattern variable */
char	*get_matching_entries(const char *pattern)
{
	t_entries_arr	entries_arr;
	t_entry			**entries;
	char			*asterisk;
	size_t			prefix_len;
	size_t			suffix_len;
	char			*prefix;
	char			*suffix;
	t_entry			**matching_entries;
	char			*ret;
	int				idx;
	char			**ret_arr;

	entries = entries_arr.entries;
	matching_entries = entries_arr.matching_entries;
	entries = build_entries_array(entries, &entries_arr.count);
	matching_entries = malloc(sizeof(t_entry *) * (entries_arr.count + 1));
	if (!matching_entries)
		return (NULL);
	idx = 0;
	while (idx < entries_arr.count)
	{
		matching_entries[idx] = malloc(sizeof(t_entry));
		if (!matching_entries[idx])
			return (NULL);
		matching_entries[idx]->entry = ft_strdup("");
		matching_entries[idx]->idx = NULL;
		idx++;
	}
	matching_entries[entries_arr.count] = NULL;
	asterisk = ft_strchr(pattern, '*');
	while (asterisk)
	{
		prefix_len = asterisk - pattern;
		prefix = ft_substr(pattern, 0, prefix_len);
		idx = 0;
		while (entries[idx])
		{
			if (ft_strncmp(entries[idx]->entry, prefix, prefix_len) == 0)
			{
				free(matching_entries[idx]->entry);
				matching_entries[idx]->entry = ft_strdup(entries[idx]->entry);
				matching_entries[idx]->idx = entries[idx]->idx + prefix_len;
			}
			idx++;
		}
		pattern = asterisk + 1;
		asterisk = ft_strchr(pattern, '*');
	}
	suffix_len = ft_strlen(pattern);
	suffix = ft_substr(pattern, 0, suffix_len);
	idx = 0;
	while (entries[idx])
	{
		if (ft_strncmp(entries[idx]->idx, suffix, suffix_len) == 0)
		{
			if (matching_entries[idx] == NULL)
				matching_entries[idx] = malloc(sizeof(t_entry));
			matching_entries[idx]->entry = ft_strdup(entries[idx]->entry);
			matching_entries[idx]->idx = entries[idx]->idx + suffix_len;
		}
		idx++;
	}
	ret_arr = entry_arr_to_char_arr(entries_arr.matching_entries,
		entries_arr.count);
	ret = ft_strjoin_arr(ret_arr, " ");
	while (matching_entries[idx])
	{
		if (matching_entries[idx]->entry)
			free(matching_entries[idx]->entry);
		matching_entries[idx]->entry = NULL;
		matching_entries[idx]->idx = NULL;
		free(matching_entries[idx]);
		matching_entries[idx] = NULL;
		idx++;
	}
	return (ret);
}

char	*wildcard_expansion(const char *input)
{
	const char *str;
	char *matched_files;
	t_pattern pattern;

	str = input;
	while (*str != '\0')
	{
		if (*str == '\'' || *str == '\"')
		{
			const char *quote = ft_strchr(str + 1, *str);
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
			matched_files = get_matching_entries(pattern.pattern);
		}
	}
	return (matched_files);
}