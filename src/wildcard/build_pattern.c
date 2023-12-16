/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pattern.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 03:00:42 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 03:46:46 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	input_start_end_raw_reduce(t_pattern *pattern,
		const char **input_asterisk, const char **input_start, t_data *data)
{
	pattern->input_pattern_start = (char *)*input_asterisk;
	while (pattern->input_pattern_start > *input_start
		&& !ft_isspace(*(pattern->input_pattern_start - 1))
		&& *(pattern->input_pattern_start - 1) != '\0')
		pattern->input_pattern_start--;
	pattern->input_pattern_end = pattern->input_pattern_start;
	while (*pattern->input_pattern_end
		&& !ft_isspace(*pattern->input_pattern_end))
		pattern->input_pattern_end++;
	pattern->pattern_raw_len = pattern->input_pattern_end
		- pattern->input_pattern_start;
	pattern->pattern_raw = ft_substr(pattern->input_pattern_start, 0,
			pattern->pattern_raw_len);
	pattern->pattern = reduce_consecutive_char(pattern->pattern_raw, '*', data);
	free(pattern->pattern_raw);
	pattern->pattern_raw = NULL;
}

void	build_single_midfix(t_pattern *p)
{
	p->start = p->asterisk_reader;
	while (*p->asterisk_reader && !ft_isspace(*p->asterisk_reader))
	{
		if (*p->asterisk_reader == '*')
		{
			p->midfix_len = p->asterisk_reader - p->start;
			p->midfixes[p->idx] = ft_substr(p->start, 0, p->midfix_len);
			p->idx++;
			break ;
		}
		p->asterisk_reader++;
	}
	if (*p->asterisk_reader != '\0')
		p->asterisk_reader++;
}

void	build_midfixes(t_pattern *p)
{
	p->asterisk_reader = (char *)p->asterisk + 1;
	while (*p->asterisk_reader && !ft_isspace(*p->asterisk_reader))
	{
		if (*p->asterisk_reader == '*')
			p->midfixes_nbr++;
		p->asterisk_reader++;
	}
	p->asterisk_reader = (char *)p->asterisk + 1;
	if (p->midfixes_nbr > 0)
	{
		p->midfixes = malloc(sizeof(char *) * (p->midfixes_nbr + 1));
		if (!p->midfixes)
			return ;
		p->midfixes[p->midfixes_nbr] = NULL;
		p->asterisk_reader = (char *)p->asterisk + 1;
		p->idx = 0;
		while (*p->asterisk_reader && !ft_isspace(*p->asterisk_reader))
			build_single_midfix(p);
	}
}

void	start_end_len_asterisk(t_pattern *pattern)
{
	pattern->start = pattern->pattern;
	pattern->end = pattern->start;
	while (*pattern->end && !ft_isspace(*pattern->end))
		pattern->end++;
	pattern->pattern_len = pattern->end - pattern->start;
	pattern->asterisk = ft_strchr(pattern->pattern, '*');
	pattern->asterisk_reader = (char *)pattern->asterisk;
	pattern->prefix_len = pattern->asterisk - pattern->start;
}

void	build_pattern(const char *input_asterisk, const char *input_start,
		t_pattern *pattern, t_data *data)
{
	init_pattern(pattern);
	input_start_end_raw_reduce(pattern, &input_asterisk, &input_start, data);
	start_end_len_asterisk(pattern);
	if (pattern->prefix_len > 0)
		pattern->prefix = ft_substr(pattern->start, 0, pattern->asterisk
				- pattern->start);
	build_midfixes(pattern);
	pattern->asterisk_reader = ft_strrchr(pattern->pattern, '*');
	if (pattern->asterisk_reader + 1)
	{
		pattern->suffix = ft_strdup(pattern->asterisk_reader + 1);
		pattern->suffix_len = ft_strlen(pattern->suffix);
	}
	else
	{
		pattern->suffix = NULL;
		pattern->suffix_len = 0;
	}
}
