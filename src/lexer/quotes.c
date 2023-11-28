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

// void	process_variable(t_var_subs *vars, t_token *token, t_data *data)
char	*process_vars_in_str(const char *str, t_data *data)
{
	char		*dollar_sign;
	char		*result;
	t_var_subs	vars;

	result = NULL;
	vars.start = str;
	vars.str = vars.start;
	while (vars.str != NULL && vars.str[0] && vars.str[0] != '\0'
		&& (ft_strchr(vars.str, '$')))
	{
		if (vars.str[1] != '?' && !ft_isvalidvarname(vars.str[1]))
			vars.str++;
		else
		{
			dollar_sign = ft_strchr(str, '$');
			vars.str = dollar_sign;
			vars.end = vars.str + 1;
			while (ft_isvalidvarname(*vars.end))
				vars.end++;
			if (*vars.end == '?')
				vars.end++;
			// The segment before the $ sign
			vars.before = ft_substr(str, 0, vars.str - str);
			if (!vars.before)
				free_var_subs_and_exit(&vars, data,
					"Error: malloc before failed\n");
			// The variable name
			vars.var_name = ft_substr(vars.str, 1, vars.end - vars.str - 1);
			if (!vars.var_name)
				free_var_subs_and_exit(&vars, data,
					"Error: malloc var_name failed\n");
			if (vars.var_name[0] == '?')
				vars.value = ft_itoa(data->last_exit_status);
			else
				vars.value = lookup_env_value(vars.var_name, data->env_arr);
			// The segment after the variable name
			vars.after = ft_strdup(vars.end);
			if (!vars.after)
				free_var_subs_and_exit(&vars, data,
					"Error: malloc after failed\n");
			vars.before_and_value = ft_strjoin(vars.before, vars.value);
			if (!vars.before_and_value)
				free_var_subs_and_exit(&vars, data,
					"Error: malloc before_and_value failed\n");
			if (result)
				free(result);
			result = ft_strjoin(vars.before_and_value, vars.after);
			if (!result)
				free_var_subs_and_exit(&vars, data,
					"Error: malloc token->str failed\n");
		}
	}
	free_var_subs(&vars);
	return (result);
}

t_lexeme	t_double_quotes_var_subs(t_token *token, t_data *data)
{
	t_lexeme	lexeme;
	char		*tmp;

	tmp = process_vars_in_str(token->str, data);
	lexeme.str = strip_quotes(tmp, data);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	free(tmp);
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
