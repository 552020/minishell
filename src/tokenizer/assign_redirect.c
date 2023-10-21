#include "minishell.h"

void	assign_redirect_in_heredoc(const char **str_ptr, t_token *token_arr,
		size_t *idx)
{
	char const	*start;

	token_arr[*idx].type = T_REDIRECT_IN;
	token_arr[*idx].str = ft_strdup("<");
	if (*(*str_ptr + 1) == '<')
	{
		token_arr[*idx].type = T_HEREDOC;
		free(token_arr[*idx].str);
		token_arr[*idx].str = ft_strdup("<<");
		(*str_ptr)++;
		while (ft_isspace(*(*str_ptr + 1)))
			(*str_ptr)++;
		if (isregularchar(*(*str_ptr + 1), *str_ptr + 1))
		{
			(*idx)++;
			(*str_ptr)++;
			start = *str_ptr;
			while (**str_ptr && isregularchar(**str_ptr, *str_ptr)
				&& !ft_isspace(**str_ptr))
				(*str_ptr)++;
			token_arr[*idx].type = T_HEREDOC_DELIMITER;
			token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
		}
		else
			ft_putendl_fd("Warning: Unexpected char", 2);
	}
}

void	assign_redirect_out_append(const char **str_ptr, t_token *token_arr,
		size_t *idx)
{
	token_arr[*idx].type = T_REDIRECT_OUT;
	token_arr[*idx].str = ft_strdup(">");
	if (*(*str_ptr + 1) == '>')
	{
		token_arr[*idx].type = T_REDIRECT_APPEND;
		free(token_arr[*idx].str);
		token_arr[*idx].str = ft_strdup(">>");
		(*str_ptr)++;
	}
}

void	assign_redirect_in_out_heredoc_append(const char **str_ptr,
		t_token *token_arr, size_t *idx)
{
	if (**str_ptr == '<')
		assign_redirect_in_heredoc(str_ptr, token_arr, idx);
	else
		assign_redirect_out_append(str_ptr, token_arr, idx);
	(*idx)++;
	(*str_ptr)++;
}