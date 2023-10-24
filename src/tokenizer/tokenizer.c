#include "minishell.h"

char	*strip_ending_trailing_spaces(char const *str)
{
	char	*trimmed;
	int		end;

	end = ft_strlen(str) - 1;
	while (end >= 0 && ft_isspace(str[end]))
		end--;
	trimmed = ft_substr(str, 0, end + 1);
	return (trimmed);
}

t_token	*create_token_array(size_t token_count)
{
	t_token	*token_arr;

	token_arr = (t_token *)ft_calloc(token_count + 1, sizeof(t_token));
	if (!token_arr)
		return (NULL);
	return (token_arr);
}

t_token	*tokenizer(t_token *token_arr, const char *str)
{
	size_t	idx;

	idx = 0;
	while (*str)
	{
		skip_spaces(&str);
		if (isregularchar(*str, str))
			assign_word(&str, token_arr, &idx);
		else if (*str == '<' || *str == '>')
			assign_redirect_in_out_heredoc_append(&str, token_arr, &idx);
		else if (*str == '|')
			assign_pipe(&str, token_arr, &idx);
		else if (*str == '$')
			assign_env_var(&str, token_arr, &idx);
		else if (*str == '\'' || *str == '"')
			assign_quotes(&str, token_arr, &idx);
		else
			handle_unexpected_char(&str);
	}
	token_arr[idx].type = T_END;
	token_arr[idx].str = NULL;
	return (token_arr);
}

void	tokenize(size_t *token_count, t_token **token_arr, char *input)
{
	char *trimmed;

	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("\n***Tokenization***\n\n");
	trimmed = strip_ending_trailing_spaces(input);
	free(input);
	*token_count = count_words_tokenizer(trimmed);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("Token count: %zu\n\n", *token_count);
	*token_arr = create_token_array(*token_count);
	*token_arr = tokenizer(*token_arr, trimmed);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		print_token_arr(*token_arr, *token_count);
}