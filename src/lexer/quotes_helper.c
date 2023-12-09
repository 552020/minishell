/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:41:23 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 17:41:26 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
str: pointer to token->str, no need to free.
end: pointer to the end of the variable name, no need to free it.
value: pointer to the value in the env_arr, no need to free it.
before: substring of token->str, has to be freed.
var_name: substring of token->str, has to be freed,
before_and_value: malloc'd string, has to be freed.
after: malloc'd string, has be freed.
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
		while (1)
		{
			tmp = ft_strchr(tmp, '$');
			if (tmp == NULL)
				break ;
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
