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
	char quote;
	char const *start;

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