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
	char		*pattern_raw;
	size_t		pattern_raw_len;
	char		*asterisk;
	char		*asterisk_reader;
	int			idx;
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

char			*wildcard_expansion(char *input, t_data *data);

void			build_pattern(const char *asterisk, const char *input_start,
					t_pattern *pattern_ptr, t_data *data);
char			*reduce_consecutive_char(const char *str, char c, t_data *data);
void			build_pattern(const char *input_asterisk,
					const char *input_start, t_pattern *pattern, t_data *data);
void			all_entries_to_matching(t_entries *entries);
void			init_pattern(t_pattern *pattern);
void			free_pattern(t_pattern *pattern);
char			**entry_to_char(t_entry **matching, t_data *data);
char			*ft_strjoin_arr(char **arr);
char			*reduce_consecutive_char(const char *str, char c, t_data *data);

#endif