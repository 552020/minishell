/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:31:03 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 17:31:07 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The first return, returns the original string it if's too short
The second return, returns a substring without the first and last characters,
which are the quotes.
*/
char	*strip_quotes(char *str, t_data *data)
{
	size_t	len;
	char	*sub_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (str);
	sub_str = ft_substr(str, 1, len - 2);
	if (!sub_str)
		free_exit(data, "Error: malloc sub_str failed\n");
	return (sub_str);
}

typedef struct s_var_subs
{
	const char	*str;
	const char	*start;
	const char	*end;
	char		*before;
	char		*var_name;
	char		*before_and_value;
	char		*value;
	char		*after;
	char		*result;
}				t_var_subs;

/*
str is just a pointer to the token->str, no need to free it.
end is a pointer to the end of the variable name, no need to free it.
value is a pointer to the value in the env_arr, no need to free it.

before is a substring of token->str, which is malloced, so it needs to be freed.
var_name is a substring of token->str, which is malloced,
	so it needs to be freed.
before_and_value is a string that is malloced, so it needs to be freed.
after is a string that is malloced, so it needs to be freed.
 */
void	free_var_subs(t_var_subs *vars)
{
	if (!vars)
		return ;
	if (vars->before)
	{
		free(vars->before);
		vars->before = NULL;
	}
	if (vars->var_name)
	{
		free(vars->var_name);
		vars->var_name = NULL;
	}
	if (vars->after)
	{
		free(vars->after);
		vars->after = NULL;
	}
	if (vars->before_and_value)
	{
		free(vars->before_and_value);
		vars->before_and_value = NULL;
	}
}

void	free_var_subs_and_exit(t_var_subs *vars, t_data *data, char *message)
{
	free_var_subs(vars);
	free_exit(data, message);
}

int	find_next_env_var_if_any(const char **str)
{
	const char	*tmp;

	tmp = *str;
	if (tmp[0] == '$' && tmp[1] != '?' && !ft_isvalidvarname(tmp[1]))
	{
		tmp++;
		while ((tmp = ft_strchr(tmp, '$')) != NULL)
		{
			if (tmp[1] != '?' && !ft_isvalidvarname(tmp[1]))
				tmp++;
			else
				break ;
		}
		if (tmp == NULL)
			return (0);
	}
	*str = tmp;
	return (1);
}

void	build_before_and_name_and_substitute_with_value(t_var_subs *vars,
		t_data *data)
{
	vars->end = vars->str + 1;
	while (ft_isvalidvarname(*vars->end))
		vars->end++;
	if (*vars->end == '?')
		vars->end++;
	vars->before = ft_substr(vars->result, 0, vars->str - vars->start);
	if (!vars->before)
		free_var_subs_and_exit(vars, data, "Error: malloc before failed\n");
	vars->var_name = ft_substr(vars->str, 1, vars->end - vars->str - 1);
	if (!vars->var_name)
		free_var_subs_and_exit(vars, data, "Error: malloc var_name failed\n");
	if (vars->var_name[0] == '?')
		vars->value = ft_itoa(data->last_exit_status);
	else
		vars->value = lookup_env_value(vars->var_name, data->env_arr);
}

char	*build_expanded_str(t_var_subs *vars, t_data *data)
{
	build_before_and_name_and_substitute_with_value(vars, data);
	vars->after = ft_strdup(vars->end);
	if (!vars->after)
		free_var_subs_and_exit(vars, data, "Error: malloc after failed\n");
	vars->before_and_value = ft_strjoin(vars->before, vars->value);
	if (!vars->before_and_value)
		free_var_subs_and_exit(vars, data,
			"Error: malloc before_and_value failed\n");
	if (vars->result)
	{
		free((vars->result));
		vars->result = NULL;
	}
	vars->result = ft_strjoin(vars->before_and_value, vars->after);
	if (!vars->result)
		free_var_subs_and_exit(vars, data, "Error: malloc token->str failed\n");
	free_var_subs(vars);
	return (vars->result);
}
char	*process_vars_in_str(const char *str, t_data *data)
{
	t_var_subs	vars;

	vars.result = ft_strdup(str);
	vars.start = vars.result;
	vars.str = vars.result;
	if (ft_strchr(vars.str, '$') == NULL)
		return (vars.result);
	else
		vars.str = ft_strchr(vars.str, '$');
	while (vars.str != NULL && vars.str[0] && vars.str[0] != '\0'
		&& (ft_strchr(vars.str, '$')))
	{
		vars.str = ft_strchr(vars.str, '$');
		if (vars.str == NULL || !find_next_env_var_if_any(&vars.str))
			break ;
		vars.str = build_expanded_str(&vars, data);
	}
	return (vars.result);
}

t_lexeme	t_double_quotes_var_subs(t_token *token, t_data *data)
{
	t_lexeme	lexeme;
	char		*tmp;

	tmp = process_vars_in_str(token->str, data);
	lexeme.str = strip_quotes(tmp, data);
	free(tmp);
	tmp = NULL;
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.type = L_UNDEFINED;
	lexeme.status = NOT_LEXED;
	lexeme.original = NULL;
	return (lexeme);
}

t_lexeme	single_quote_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_UNDEFINED;
	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.str = strip_quotes(token->str, data);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}
