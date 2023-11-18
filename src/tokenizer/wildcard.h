#ifndef WILDCARD_H
# define WILDCARD_H

# include <dirent.h>

typedef struct s_pattern
{
	char		*pattern;
	char		*prefix;
	char		*suffix;
	char		**midfixes;
	size_t		pattern_len;
	size_t		prefix_len;
	size_t		suffix_len;
	size_t		midfixes_nbr;
	size_t		midfix_len;
	char		*input_pattern_start;
	char		*input_pattern_end;
	const char	*start;
	const char	*end;
	char		*ret;
	char		*tmp;
}				t_pattern;

typedef struct s_entry
{
	char		*entry;
	char		*idx;
}				t_entry;

typedef struct s_entries
{
	t_entry		**entries;
	t_entry		**matching;
	int			count;
}				t_entries;

char			*wildcard_expansion(char *input);

void			build_pattern(const char *asterisk, const char *input_start,
					t_pattern *pattern_ptr);
char			*reduce_consecutive_char(const char *str, char c);

#endif