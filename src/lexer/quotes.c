#include "minishell.h"

char	*strip_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (str);
	// Return the original string if it's too short
	return (ft_substr(str, 1, len - 2));
	// Return a substring without the first and last characters
}

t_lexeme	t_double_quote_var_substitution(t_token *token, char **envp)
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
	lexeme.original = ft_strdup(token->str);
	while ((str = ft_strchr(str, '$')))
	{
		end = str + 1;
		while (ft_isvalidvarname(*end))
			end++;
		before = ft_substr(token->str, 0, str - token->str);
		var_name = ft_substr(str, 1, end - str - 1);
		value = lookup_env_value(var_name, envp);
		after = ft_strdup(end);
		free(token->str);
		before_and_value = ft_strjoin(before, value);
		free(before);
		token->str = ft_strjoin(before_and_value, after);
		free(before_and_value);
		free(var_name);
		free(after);
		str = token->str + (str - token->str) + ft_strlen(value);
	}
	lexeme.str = strip_quotes(token->str);
	lexeme.type = L_UNDEFINED;
	lexeme.status = NOT_LEXED;
	// printf("Lexeme: type=%d, value=%s, original=%s\n", lexeme.type,
	// lexeme.str,
	// 	lexeme.original);
	// printf("Done double quote var substitution.\n\n");
	return (lexeme);
}

t_lexeme	single_quote_lexeme(t_token *token)
{
	t_lexeme lexeme;

	lexeme.type = L_UNDEFINED;
	lexeme.original = ft_strdup(token->str);
	lexeme.str = strip_quotes(token->str);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}