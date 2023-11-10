#include "minishell.h"

char	*strip_quotes(char *str, t_data *data)
{
	size_t	len;
	char 	*sub_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (str);
	// Return the original string if it's too short
	sub_str = ft_substr(str, 1, len - 2);
	if (!sub_str)
		free_exit(data, "Error: malloc sub_str failed\n");
	return (sub_str);
	// Return a substring without the first and last characters
}

t_lexeme	t_double_quotes_var_subs(t_token *token, t_data *data)
{
	char		*str;
	char		*end;
	char		*before;
	char		*var_name;
	char		*before_and_value;
	char		*value;
	char		*after;
	t_lexeme	lexeme;

	str = token->str;
	lexeme.str = ft_strdup("");
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	while ((str = ft_strchr(str, '$')))
	{
		end = str + 1;
		while (ft_isvalidvarname(*end))
			end++;
		before = ft_substr(token->str, 0, str - token->str);
		if (!before)
			free_exit(data, "Error: malloc before failed\n");
		var_name = ft_substr(str, 1, end - str - 1);
		if (!var_name)
		{
			free(before);
			free_exit(data, "Error: malloc var_name failed\n");
		}
		value = lookup_env_value(var_name, data->env_arr);
		after = ft_strdup(end);
		if (!after)
		{
			free(before);
			free(var_name);
			free_exit(data, "Error: malloc after failed\n");
		}
		free(token->str);
		before_and_value = ft_strjoin(before, value);
		if (!before_and_value)
		{
			free(before);
			free(var_name);
			free(after);
			free_exit(data, "Error: malloc before_and_value failed\n");
		}
		free(before);
		token->str = ft_strjoin(before_and_value, after);
		if (!token->str)
		{
			free(before_and_value);
			free(var_name);
			free(after);
			free_exit(data, "Error: malloc token->str failed\n");
		}
		free(before_and_value);
		free(var_name);
		free(after);
		str = token->str + (str - token->str) + ft_strlen(value);
	}
	lexeme.str = strip_quotes(token->str, data);
	lexeme.type = L_UNDEFINED;
	lexeme.status = NOT_LEXED;
	// printf("Lexeme: type=%d, value=%s, original=%s\n", lexeme.type,
	// lexeme.str,
	// 	lexeme.original);
	// printf("Done double quote var substitution.\n\n");
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