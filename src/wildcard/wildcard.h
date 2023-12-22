/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 05:19:01 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 05:25:11 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int			asterisks_to_skip;
}				t_wildcard;


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
void			init_entries(t_entries *entries_ptr);
void			free_entries(t_entries *entries);
char			*get_matching_entries(t_pattern *pattern, t_data *data, t_wildcard *wildcard);
void			init_matching(t_entries *entries);
void			check_suffix(t_entries *entries, t_pattern *pattern);

void			free_get_matching_entries(t_entries *entries, char ***ret_arr,
					t_pattern *pattern);

#endif
