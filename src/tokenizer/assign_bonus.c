#include "minishell.h"
#include "tokenizer.h"

void	assign_parentheses_open(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_PARENTHESES_OPEN;
	(*idx)++;
	(*str)++;
}

void	assign_parentheses_close(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_PARENTHESES_CLOSE;
	(*idx)++;
	(*str)++;
}

void	assign_log_and(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_LOG_AND;
	(*idx)++;
	(*str) += 2;
}

void	assign_log_or(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_LOG_OR;
	(*idx)++;
	(*str) += 2;
}
