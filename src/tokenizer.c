#include "minishell.h"

int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int	isspecialchar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '$' || c == '"' || c == '\'')
		return (1);
	return (0);
}

int	isregularchar(char c, char *str)
{
	if (ft_isspace(c) || isspecialchar(c))
		return (0);
	if (c == '\'' || c == '"')
	{
		if (!ft_strrchr(str, c))
			return (1);
		return (0);
	}
	return (1);
}

size_t	count_words_tokenizer(const char *input)
{
	size_t	words;
	char	*str;
	char	quote;

	str = (char *)input;
	words = 0;
	while (*str && ft_isspace(*str))
		str++;
	while (*str)
	{
		if (isspecialchar(*str))
		{
			words++;
			if (*str == '$')
			{
				str++;
				while (*str && ft_isvalidvarname(*str))
					str++;
			}
			else if ((*str == '<' || *str == '>') && *(str + 1) == *str)
				str++;
			else if (*str == '\'' || *str == '"')
			{
				quote = *str;
				str++;
				while (*str && *str != quote)
					str++;
				str++;
			}
		}
		else if (ft_isspace(*str))
		{
			while (*str && ft_isspace(*str))
				str++;
		}
		else if (isregularchar(*str, str))
		{
			words++;
			while (*str && isregularchar(*str, str))
				str++;
		}
		else
		{
			ft_putendl_fd("Warning: Unexpected character encountered during tokenization.",
				STDERR_FILENO);
			str++;
		}
		if (*str != '\0')
			str++;
	}
	return (words);
}

t_token	*tokenizer(const char *input)
{
	t_token *token_arr;
	char *str;
	size_t idx;

	token_arr = (t_token *)malloc(sizeof(t_token)
			* (count_words_tokenizer(input) + 1));
	ft_memset(token_arr, 0, sizeof(t_token) * (count_words_tokenizer(input)
			+ 1));
	if (!token_arr)
		return (NULL);
	str = (char *)input;
	idx = 0;
	while (*str)
	{
		if (ft_isspace(*str))
			str++;
		else if (isregularchar(*str, str))
		{
			char *start = str;
			while (*str && isregularchar(*str, str))
				str++;

			token_arr[idx].type = T_WORD;
			token_arr[idx].str = ft_substr(start, 0, str - start);
			idx++;
		}
		else if (*str == '<' || *str == '>')
		{
			if (*str == '<')
			{
				token_arr[idx].type = T_REDIRECT_IN;
				token_arr[idx].str = ft_strdup("<");
				if (*(str + 1) == '<')

				{
					token_arr[idx].type = T_HEREDOC;
					free(token_arr[idx].str);
					token_arr[idx].str = ft_strdup("<<");
					str++;
					while (ft_isspace(*(str + 1)))
						str++;
					if (isregularchar(*(str + 1), str + 1))
					{
						idx++;
						char *start = str + 1;
						while (*str && isregularchar(*str, str)
							&& !ft_isspace(*str))
							str++;

						token_arr[idx].type = T_HEREDOC_DELIMITER;
						token_arr[idx].str = ft_substr(start, 0, str - start);
					}
					else
					{
						ft_putendl_fd("Warning: Unexpected character encountered during tokenization of heredoc delimiter.",
							STDERR_FILENO);
					}
				}
			}
			else
			{
				token_arr[idx].type = T_REDIRECT_OUT;
				token_arr[idx].str = ft_strdup(">");
				if (*(str + 1) == '>')
				{
					token_arr[idx].type = T_REDIRECT_APPEND;
					free(token_arr[idx].str);
					token_arr[idx].str = ft_strdup(">>");
					str++;
				}
			}
			idx++;
			str++;
		}
		else if (*str == '|')
		{
			token_arr[idx].type = T_PIPE;
			token_arr[idx].str = ft_strdup("|");
			idx++;
			str++;
		}
		else if (*str == '$')
		{
			char *start = str;
			str++; // Move past the $
			while (*str && ft_isvalidvarname(*str))
				str++;

			token_arr[idx].type = T_ENV_VAR;
			token_arr[idx].str = ft_substr(start, 0, str - start);
			idx++;
		}
		else if (*str == '\'' || *str == '"')
		{
			char quote = *str;
			char *start = str;
			str++; // Move past the current quote
			while (*str && *str != quote)
				str++;

			if (quote == '\'')
			{
				token_arr[idx].type = T_SINGLE_QUOTE;
			}
			else
			{
				token_arr[idx].type = T_DOUBLE_QUOTE;
			}

			token_arr[idx].str = ft_substr(start, 0, str - start + 1);
			idx++;
			str++;
		}
		else
		{
			ft_putendl_fd("Warning: Unexpected character encountered during tokenization.",
				STDERR_FILENO);
			str++;
		}
	}
	token_arr[idx].type = T_END;
	token_arr[idx].str = NULL;

	return (token_arr);
}