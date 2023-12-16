/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 20:06:58 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 05:26:11 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

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

void	free_t_wildcard(t_wildcard *vars)
{
	if (vars->before)
	{
		free(vars->before);
		vars->before = NULL;
	}
	if (vars->after)
	{
		free(vars->after);
		vars->after = NULL;
	}
	if (vars->tmp)
	{
		free(vars->tmp);
		vars->tmp = NULL;
	}
	if (vars->matched_files)
	{
		free(vars->matched_files);
		vars->matched_files = NULL;
	}
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
	free_t_wildcard(vars);
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
