#include "minishell.h"

void	assign_word(const char **str_ptr, t_data *data, size_t *idx)
{
	char	*start;

	start = (char *)*str_ptr;
	while (**str_ptr && isregularchar(**str_ptr, *str_ptr))
		(*str_ptr)++;
	data->token_arr[*idx].type = T_WORD;
	data->token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
	if (data->token_arr[*idx].str == NULL)
		free_exit(data, "Error: ft_substr failed\n");
	(*idx)++;
}

void	assign_pipe(const char **str_ptr, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_PIPE;
	data->token_arr[*idx].str = ft_strdup("|");
	if (!data->token_arr[*idx].str)
		free_exit(data, "Error: ft_strdup failed\n");
	(*idx)++;
	(*str_ptr)++;
}

void	assign_env_var(const char **str_ptr, t_data *data, size_t *idx)
{
	char const	*start;

	start = *str_ptr;
	(*str_ptr)++;
	while (**str_ptr && ft_isvalidvarname(**str_ptr))
		(*str_ptr)++;
	data->token_arr[*idx].type = T_ENV_VAR;
	data->token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
	if (!data->token_arr[*idx].str)
		free_exit(data, "Error: ft_substr failed\n");
	(*idx)++;
}

void	assign_quotes(const char **str_ptr, t_data *data, size_t *idx)
{
	char quote;
	char const *start;

	quote = **str_ptr;
	start = *str_ptr;
	(*str_ptr)++;
	while (**str_ptr && **str_ptr != quote)
		(*str_ptr)++;
	if (quote == '\'')
		data->token_arr[*idx].type = T_SINGLE_QUOTE;
	else
		data->token_arr[*idx].type = T_DOUBLE_QUOTE;
	data->token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start + 1);
	if (!data->token_arr[*idx].str)
		free_exit(data, "Error: ft_substr failed\n");
	(*idx)++;
	(*str_ptr)++;
}