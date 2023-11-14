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
	char	*str;
	char	*end;
	char	*before;
	char	*var_name;
	char	*before_and_value;
	char	*value;
	char	*after;
}			t_var_subs;

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
		free(vars->before);
	if (vars->var_name)
		free(vars->var_name);
	if (vars->after)
		free(vars->after);
	if (vars->before_and_value)
		free(vars->before_and_value);
}

void	free_var_subs_and_exit(t_var_subs *vars, t_data *data, char *message)
{
	free_var_subs(vars);
	free_exit(data, message);
}

void	process_variable(t_var_subs *vars, t_token *token, t_data *data)
{
	vars->end = vars->str + 1;
	while (ft_isvalidvarname(*vars->end))
		vars->end++;
	vars->before = ft_substr(token->str, 0, vars->str - token->str);
	if (!vars->before)
		free_var_subs_and_exit(vars, data, "Error: malloc before failed\n");
	vars->var_name = ft_substr(vars->str, 1, vars->end - vars->str - 1);
	if (!vars->var_name)
		free_var_subs_and_exit(vars, data, "Error: malloc var_name failed\n");
	vars->value = lookup_env_value(vars->var_name, data->env_arr);
	vars->after = ft_strdup(vars->end);
	if (!vars->after)
		free_var_subs_and_exit(vars, data, "Error: malloc after failed\n");
	free(token->str);
	vars->before_and_value = ft_strjoin(vars->before, vars->value);
	if (!vars->before_and_value)
		free_var_subs_and_exit(vars, data,
			"Error: malloc before_and_value failed\n");
	token->str = ft_strjoin(vars->before_and_value, vars->after);
	if (!token->str)
		free_var_subs_and_exit(vars, data, "Error: malloc token->str failed\n");
	free_var_subs(vars);
}

t_lexeme	t_double_quotes_var_subs(t_token *token, t_data *data)
{
	t_var_subs	vars;
	t_lexeme	lexeme;

	vars.str = token->str;
	lexeme.str = ft_strdup("");
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	while ((vars.str = ft_strchr(vars.str, '$')))
	{
		process_variable(&vars, token, data);
		vars.str = token->str + (vars.str - token->str) + ft_strlen(vars.value);
	}
	lexeme.str = strip_quotes(token->str, data);
	lexeme.type = L_UNDEFINED;
	lexeme.status = NOT_LEXED;
	return (lexeme);
}

t_lexeme	single_quote_lexeme(t_token *token, t_data *data)
{
	t_lexeme lexeme;

	lexeme.type = L_UNDEFINED;
	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.str = strip_quotes(token->str, data);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}