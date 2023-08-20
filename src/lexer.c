#include "minishell.h"

char	*lookup_env_value(char *var_name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1); // Return the value part after the '='
		i++;
	}
	return (""); // bash returns an empty string if the variable is not found
}

t_lexeme	t_env_var_substitution(t_token *token, char **envp)
{
	t_lexeme	lexeme;
	char		*value;

	lexeme.original = ft_strdup(token->str);
	value = lookup_env_value(token->str + 1, envp); //
	// +1 to skip the '$' if (value)
	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(value);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}

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
	return (lexeme);
}

t_lexeme	single_quote_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	pipe_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_PIPE;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_in_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_INPUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_in_target_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDIN;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_OUTPUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_target_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDOUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_append_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_APPEND;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	heredoc_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_HEREDOC;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	heredoc_delimiter_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_HEREDOC_DELIMITER;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	heredoc_content_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_HEREDOC_CONTENT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	*lexer(t_token *token_arr, char **envp, size_t token_count)
{
	size_t		i;
	t_lexeme	*lexeme_arr;

	lexeme_arr = malloc(sizeof(t_lexeme) * (token_count + 1));
	i = 0;
	while (i < token_count + 1)
	{
		if (token_arr[i].type == T_ENV_VAR)
			t_env_var_substitution(&token_arr[i], envp);
		// token_arr[i] = t_env_var_substitution(&token_arr[i], envp);
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
			t_double_quote_var_substitution(&token_arr[i], envp);
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			single_quote_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_PIPE)
			pipe_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_REDIRECT_IN)
		{
			redirect_in_lexeme(&token_arr[i]);
			redirect_in_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_REDIRECT_OUT)
		{
			redirect_out_lexeme(&token_arr[i]);
			redirect_out_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_REDIRECT_APPEND)
		{
			redirect_append_lexeme(&token_arr[i]);
			redirect_out_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_HEREDOC)
		{
			heredoc_lexeme(&token_arr[i]);
			heredoc_delimiter_lexeme(&token_arr[i + 1]);
			heredoc_content_lexeme(&token_arr[i + 2]);
		}
		else
			;
		i++;
	}
	return (lexeme_arr);
}
