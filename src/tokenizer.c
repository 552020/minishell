#include "minishell.h"

void	assign_word(const char **str_ptr, t_token *token_arr, size_t *idx)
{
	char	*start;

	start = (char *)*str_ptr;
	while (**str_ptr && isregularchar(**str_ptr, *str_ptr))
		(*str_ptr)++;
	token_arr[*idx].type = T_WORD;
	token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
	(*idx)++;
}

void	assign_pipe(const char **str_ptr, t_token *token_arr, size_t *idx)
{
	token_arr[*idx].type = T_PIPE;
	token_arr[*idx].str = ft_strdup("|");
	(*idx)++;
	(*str_ptr)++;
}

void	assign_env_var(const char **str_ptr, t_token *token_arr, size_t *idx)
{
	char const	*start;

	start = *str_ptr;
	(*str_ptr)++;
	while (**str_ptr && ft_isvalidvarname(**str_ptr))
		(*str_ptr)++;
	token_arr[*idx].type = T_ENV_VAR;
	token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
	(*idx)++;
}

void	assign_quotes(const char **str_ptr, t_token *token_arr, size_t *idx)
{
	char		quote;
	char const	*start;

	quote = **str_ptr;
	start = *str_ptr;
	(*str_ptr)++;
	while (**str_ptr && **str_ptr != quote)
		(*str_ptr)++;
	if (quote == '\'')
		token_arr[*idx].type = T_SINGLE_QUOTE;
	else
		token_arr[*idx].type = T_DOUBLE_QUOTE;
	token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start + 1);
	(*idx)++;
	(*str_ptr)++;
}

t_token	*tokenizer(const char *str, size_t token_count)
{
	t_token *token_arr;
	size_t idx;

	token_arr = (t_token *)malloc(sizeof(t_token) * (token_count + 1));
	ft_memset(token_arr, 0, sizeof(t_token) * (token_count + 1));
	if (!token_arr)
		return (NULL);
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
		{
			ft_putendl_fd("Warning: Unexpected character  during tokenization.",
				STDERR_FILENO);
			str++;
		}
	}
	token_arr[idx].type = T_END;
	token_arr[idx].str = NULL;

	return (token_arr);
}