/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 13:10:30 by slombard          #+#    #+#             */
/*   Updated: 2023/12/16 00:08:29 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

void	assign_parentheses_open(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_PARENTHESIS_OPEN;
	(*idx)++;
	(*str)++;
}

void	assign_parentheses_close(const char **str, t_data *data, size_t *idx)
{
	data->token_arr[*idx].type = T_PARENTHESIS_CLOSED;
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
