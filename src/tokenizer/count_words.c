#include "minishell.h"

void	count_word_special_char(const char **str_ptr, size_t *words)
{
	char	quote;

	if (isspecialchar(**str_ptr))
	{
		(*words)++;
		if (**str_ptr == '$')
		{
			(*str_ptr)++;
			while (**str_ptr && ft_isvalidvarname(**str_ptr))
				(*str_ptr)++;
		}
		else if ((**str_ptr == '<' || **str_ptr == '>') && *(*str_ptr
				+ 1) == **str_ptr)
			(*str_ptr)++;
		else if (**str_ptr == '\'' || **str_ptr == '"')
		{
			quote = **str_ptr;
			(*str_ptr)++;
			while (**str_ptr && **str_ptr != quote)
				(*str_ptr)++;
			(*str_ptr)++;
		}
		else
			(*str_ptr)++;
	}
}

size_t	count_words_tokenizer(const char *str)
{
	size_t words;

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
	}
	return (words);
}