#include "minishell.h"

size_t	count_words_tokenizer(const char *str)
{
	size_t	words;

	words = 0;
	while (*str)
	{
		skip_spaces(&str);
		if (isspecialchar(*str))
			count_word_special_char(&str, &words);
		else if (isregularchar(*str, str))
		{
			words++;
			while (*str && isregularchar(*str, str))
				str++;
		}
		else
		{
			ft_putendl_fd("Warning: Unexpected char.", STDERR_FILENO);
			str++;
		}
		if (*str != '\0')
			str++;
	}
	return (words);
}

t_token	*tokenizer(const char *input, size_t token_count)
{
	t_token *token_arr;
	char *str;
	size_t idx;

	token_arr = (t_token *)malloc(sizeof(t_token) * (token_count + 1));
	ft_memset(token_arr, 0, sizeof(t_token) * (token_count + 1));
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