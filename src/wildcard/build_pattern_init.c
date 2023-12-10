/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pattern_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 03:46:18 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 03:47:29 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

void	init_pattern(t_pattern *pattern)
{
	pattern->pattern = NULL;
	pattern->prefix = NULL;
	pattern->suffix = NULL;
	pattern->midfixes = NULL;
	pattern->pattern_len = 0;
	pattern->prefix_len = 0;
	pattern->suffix_len = 0;
	pattern->midfixes_nbr = 0;
	pattern->midfix_len = 0;
	pattern->input_pattern_start = NULL;
	pattern->input_pattern_end = NULL;
	pattern->start = NULL;
	pattern->end = NULL;
	pattern->ret = NULL;
	pattern->tmp = NULL;
	pattern->pattern_raw = NULL;
	pattern->pattern_raw_len = 0;
	pattern->asterisk = NULL;
	pattern->asterisk_reader = NULL;
	pattern->idx = 0;
}
