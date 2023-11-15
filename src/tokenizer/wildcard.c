#include "minishell.h"
#include "wildcard.h"
#include <dirent.h>

typedef struct s_pattern
{
	const char	*start;
	const char	*end;
	char		*ret;
	char		*prefix;
	char		*suffix;
	char		*tmp;
}				t_pattern;

char	*build_pattern(const char *asterisk, const char *input_start)
{
	t_pattern	pattern;

	pattern.prefix = NULL;
	pattern.suffix = NULL;
	/* while:
	- we are not regressing beyond the start of the input
	- the character before is not a space
	-  the character before is not the null terminator
	*/
	while (pattern.start > input_start && !ft_isspace(*(pattern.start - 1))
		&& *(pattern.start - 1) != '\0')
		pattern.start--;
	// if there is a prefix, i.e. if start and asterisk are not the same
	if (pattern.start != asterisk)
		pattern.prefix = ft_substr(pattern.start, 0, asterisk - pattern.start);
	while (*pattern.end && !ft_isspace(*pattern.end))
		pattern.end++;
	/*
	In this case we check asterisk
	+ 1 because the string 'pattern.end' starts being the asterisk  so the while will always be true the first time
	*/
	if (pattern.end != asterisk + 1)
		pattern.suffix = ft_substr(asterisk + 1, 0, pattern.end - asterisk);
	pattern.tmp = ft_strjoin(pattern.prefix, "*");
	pattern.ret = ft_strjoin(pattern.tmp, pattern.suffix);
	free(pattern.tmp);
	free(pattern.prefix);
	free(pattern.suffix);
	return (pattern.ret);
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
char	*reduce_consecutive_chard(const char *str, char c)
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
	// We use this pointer so that 'str' is always pointing to the beginning of the str.
	// Same for this one.
	while (*src_ptr)
	{
		*dest_ptr = *src_ptr;
		if (src_ptr == c)
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

typedef struct s_entry
{
	char		*entry;
	char		*idx;
}				t_entry;

typedef struct s_entries_arr
{
	t_entry		**entries;
	int			count;
}				t_entries_arr;

t_entry	*build_entries_array(t_entry **entries, int *count)
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

	entries = entries_arr.entries;
	entries = build_entries_array(entries, &entries_arr.count);
	asterisk = ft_strchr(pattern, '*');
	matching_entries = malloc(sizeof(t_entry *) * (entries_arr.count + 1));
	if (!matching_entries)
		return (NULL);
	idx = 0;
	while (matching_entries[idx])
	{
		matching_entries[idx] = malloc(sizeof(t_entry));
		if (!matching_entries[idx])
			return (NULL);
		matching_entries[idx] = ft_strdup("");
		idx++;
	}
	matching_entries[entries_arr.count] = NULL;
	while (asterisk)
	{
		prefix_len = asterisk - pattern;
		prefix = ft_substr(pattern, 0, prefix_len);
		idx = 0;
		while (entries[idx])
		{
			if (ft_strncmp(entries[idx]->idx, prefix, prefix_len) == 0)
			{
				if (matching_entries[idx] == NULL)
					matching_entries[idx] = malloc(sizeof(t_entry));
				matching_entries[idx]->entry = ft_strdup(entries[idx]->entry);
				matching_entries[idx]->idx = entries[idx]->idx + prefix_len;
			}
			idx++;
		}
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
	ret = ft_strjoin_arr(matching_entries, " ");
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
	const char *pattern;
	const char *ret;

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
			pattern = build_pattern(str, input);
			// free pattern later on eventually
			char *matched_files = get_matching_entries(pattern);
		}
	}
}