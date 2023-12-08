/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 05:20:53 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 05:27:34 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_here_doc_delimiter(const char **str_ptr, t_data *data,
		size_t *idx)
{
	char const	*start;

	(*idx)++;
	(*str_ptr)++;
	start = *str_ptr;
	while (**str_ptr && isregularchar(**str_ptr, *str_ptr)
		&& !ft_isspace(**str_ptr))
		(*str_ptr)++;
	data->token_arr[*idx].type = T_HEREDOC_DELIMITER;
	data->token_arr[*idx].str = ft_substr(start, 0, *str_ptr - start);
	if (data->token_arr[*idx].str == NULL)
		free_exit(data, "Error: ft_substr failed\n");
}

void	assign_redirect_in_heredoc(const char **str_ptr, t_data *data,
		size_t *idx)
{
	data->token_arr[*idx].type = T_REDIRECT_IN;
	data->token_arr[*idx].str = ft_strdup("<");
	if (!data->token_arr[*idx].str)
		free_exit(data, "Error: ft_strdup failed\n");
	if (*(*str_ptr + 1) == '<')
	{
		data->token_arr[*idx].type = T_HEREDOC;
		free(data->token_arr[*idx].str);
		data->token_arr[*idx].str = NULL;
		data->token_arr[*idx].str = ft_strdup("<<");
		if (!data->token_arr[*idx].str)
			free_exit(data, "Error: ft_strdup failed\n");
		(*str_ptr)++;
		while (ft_isspace(*(*str_ptr + 1)))
			(*str_ptr)++;
		if (isregularchar(*(*str_ptr + 1), *str_ptr + 1) && (*(*str_ptr + 1)))
			assign_here_doc_delimiter(str_ptr, data, idx);
		else
			ft_putendl_fd("Warning: Unexpected char", 2);
	}
}

void	assign_redirect_out_append(const char **str_ptr, t_data *data,
		size_t *idx)
{
	data->token_arr[*idx].type = T_REDIRECT_OUT;
	data->token_arr[*idx].str = ft_strdup(">");
	if (!data->token_arr[*idx].str)
		free_exit(data, "Error: ft_strdup failed\n");
	if (*(*str_ptr + 1) == '>')
	{
		data->token_arr[*idx].type = T_REDIRECT_APPEND;
		free(data->token_arr[*idx].str);
		data->token_arr[*idx].str = NULL;
		data->token_arr[*idx].str = ft_strdup(">>");
		if (!data->token_arr[*idx].str)
			free_exit(data, "Error: ft_strdup failed\n");
		(*str_ptr)++;
	}
}

void	assign_redirect_in_out_heredoc_append(const char **str_ptr,
		t_data *data, size_t *idx)
{
	if (**str_ptr == '<')
		assign_redirect_in_heredoc(str_ptr, data, idx);
	else
		assign_redirect_out_append(str_ptr, data, idx);
	if (*str_ptr && **str_ptr)
		(*str_ptr)++;
	(*idx)++;
}
