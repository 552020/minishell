#include "minishell.h"

int	ft_isspace(int c)
{
	// ' ' (space) is 32 in ASCII
	// '\t' (horizontal tab) is 9 in ASCII
	// '\n' (newline) is 10 in ASCII
	// '\v' (vertical tab) is 11 in ASCII
	// '\f' (form feed) is 12 in ASCII
	// '\r' (carriage return) is 13 in ASCII
	if (c == 32 || (c >= 9 && c <= 13))
		return (1); // Indicates success (it's a whitespace)
	else
		return (0); // Indicates failure (it's not a whitespace)
}

/* Check all special chars in the scope of the minishell*/
int	ft_isspecial(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '$' || c == '"' || c == '\'')
		return (1);
	return (0);
}

/* Check if a normal char in a word*/
int	ft_isregularwordchar(char c, char *str)
{
	if (ft_isspace(c) || ft_isspecial(c))
		return (0);
	if (c == '\'' || c == '"')
	{
		// If there's no matching quote, treat it as a regular character
		if (!ft_strrchr(str, c))
			return (1);
		return (0); // Else, it's not a regular word character
	}
	return (1);
	// All other characters are considered regular word characters
}

size_t	count_words_tokenizer(const char *input)
{
	size_t	words;
	char	*str;
	char	quote;

	str = (char *)input;
	words = 0;
	// Skip initial spaces
	while (*str && ft_isspace(*str))
		str++;
	while (*str)
	{
		if (ft_isspecial(*str))
		{
			words++;
			if ((*str == '<' || *str == '>') && *(str + 1) == *str)
				str++;
			if (*str == '\'' || *str == '"')
			{
				quote = *str;
				str++; // Move past the current quote
				while (*str && *str != quote)
					str++;
			}
			str++;
		}
		else if (ft_isspace(*str))
		{
			while (*str && ft_isspace(*str)) // Skip spaces
				str++;
		}
		else if (ft_isregularwordchar(*str, str))
		{
			words++;
			while (*str && ft_isregularwordchar(*str, str))
				str++;
			continue ; // To prevent str++ at the end
		}
		else
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
	if (!token_arr)
		return (NULL);
	str = (char *)input;
	idx = 0;
	while (*str)
	{
		if (ft_isspace(*str))
			str++;
		else if (ft_isregularwordchar(*str, str))
		{
			char *start = str;
			while (*str && ft_isregularwordchar(*str, str))
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
					token_arr[idx].type = T_REDIRECT_HEREDOC;
					token_arr[idx].str = ft_strdup("<<");
					str++;
				}
			}
			else // if (*str == '>')
			{
				token_arr[idx].type = T_REDIRECT_OUT;
				token_arr[idx].str = ft_strdup(">");
				if (*(str + 1) == '>')
				{
					token_arr[idx].type = T_REDIRECT_APPEND;
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
	// Finalization
	token_arr[idx].type = 0; // To mark the end of the token array
	token_arr[idx].str = NULL;

	return (token_arr);
}