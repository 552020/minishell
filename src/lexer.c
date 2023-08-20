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

	printf("Double quote var substitution...\n");
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
	printf("Lexeme: type=%d, value=%s, original=%s\n", lexeme.type, lexeme.str,
		lexeme.original);
	printf("Done double quote var substitution.\n");
	return (lexeme);
}

t_lexeme	single_quote_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_UNDEFINED;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = NOT_LEXED;
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

/* To this point we don't have a delimiter token anymore */
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

t_lexeme	word_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_UNDEFINED;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}

t_lexeme	*lexer(t_token *token_arr, char **envp, size_t token_count)
{
	size_t		i;
	t_lexeme	*lexeme_arr;
	int			command_flag;

	printf("Lexing...\n");
	printf("First round of lexing...\n");
	lexeme_arr = malloc(sizeof(t_lexeme) * (token_count + 1));
	i = 0;
	while (i < token_count)
	{
		printf("Token %zu: type=%d, str=%s\n", i, token_arr[i].type,
			token_arr[i].str);
		i++;
	}
	i = 0;
	while (i < token_count)
	{
		if (token_arr[i].type == T_ENV_VAR)
			lexeme_arr[i] = t_env_var_substitution(&token_arr[i], envp);
		// token_arr[i] = t_env_var_substitution(&token_arr[i], envp);
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
		{
			lexeme_arr[i] = t_double_quote_var_substitution(&token_arr[i],
				envp);
			printf("Lexeme: type=%d, value=%s\n", lexeme_arr[i].type,
				lexeme_arr[i].str);
		}
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			lexeme_arr[i] = single_quote_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_PIPE)
			lexeme_arr[i] = pipe_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_REDIRECT_IN)
		{
			lexeme_arr[i] = redirect_in_lexeme(&token_arr[i]);
			lexeme_arr[i + 1] = redirect_in_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_REDIRECT_OUT)
		{
			lexeme_arr[i] = redirect_out_lexeme(&token_arr[i]);
			lexeme_arr[i + 1] = redirect_out_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_REDIRECT_APPEND)
		{
			lexeme_arr[i] = redirect_append_lexeme(&token_arr[i]);
			lexeme_arr[i + 1] = redirect_out_target_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_HEREDOC)
		{
			lexeme_arr[i] = heredoc_lexeme(&token_arr[i]);
			lexeme_arr[i + 1] = heredoc_content_lexeme(&token_arr[i + 1]);
		}
		else if (token_arr[i].type == T_WORD)
			lexeme_arr[i] = lexeme_arr[i] = word_lexeme(&token_arr[i]);
		else
			;
		i++;
	}
	i = 0;
	printf("\nLexeme array after first loop:\n");
	while (i < token_count)
	{
		printf("Token %zu: type=%d, str=%s\n", i, lexeme_arr[i].type,
			lexeme_arr[i].str);
		i++;
	}
	printf("Second round of lexing...\n");
	i = 0;
	// 0 means we haven't encountered a command yet, 1 means we have
	command_flag = 0;
	while (i < token_count)
	{
		printf("Lexeme %zu: type=%d, str=%s\n", i, lexeme_arr[i].type,
			lexeme_arr[i].str);
		if (lexeme_arr[i].type == L_UNDEFINED)
		{
			if (command_flag == 0)
			{
				lexeme_arr[i].type = L_COMMAND;
				command_flag = 1;
			}
			else
			{
				lexeme_arr[i].type = L_ARGUMENT;
			}
		}
		else if (lexeme_arr[i].type == L_PIPE)
		{
			command_flag = 0; // Reset the command flag when encountering a pipe
		}
		i++;
	}
	printf("Done lexing.\n\n\n");
	return (lexeme_arr);
}
