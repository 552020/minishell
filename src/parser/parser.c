/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:15:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/22 06:01:38 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data)
{
	data->ast_root = parser(data->lexeme_arr, 0, data->token_count - 1, data);
	free_lexeme_arr(data);
}

t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	int			i;
	t_ast_node	*node;

	node = NULL;
	i = end;
	while (i >= start)
	{
		if (lexemes[i].type == L_PIPE)
		{
			node = create_node(N_PIPE, data);
			node->children[1] = build_cmd_node(lexemes, i + 1, end, data);
			end = i - 1;
			i = end;
			while (i >= start && lexemes[i].type != L_PIPE)
				i--;
			if (i > start)
				node->children[0] = parser(lexemes, start, end, data);
			else
				node->children[0] = build_cmd_node(lexemes, start, end, data);
			return (node);
		}
		i--;
	}
	node = build_cmd_node(lexemes, start, end, data);
	return (node);
}

void	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_cmd_node(t_ast_node *node)
{
	if (node->cmd)
	{
		free(node->cmd);
		node->cmd = NULL;
	}
	if (node->args)
		free_str_arr(node->args);
	if (node->input_files)
		free_str_arr(node->input_files);
	if (node->output_files)
		free_str_arr(node->output_files);
	if (node->heredoc_del)
	{
		free(node->heredoc_del);
		node->heredoc_del = NULL;
	}
}

void	free_ast(t_ast_node *node)
{
	if (node->type == N_COMMAND)
	{
		free_cmd_node(node);
	}
	else if (node->type == N_PIPE)
	{
		free_ast(node->children[0]);
		free_ast(node->children[1]);
	}
	if (node)
	{
		free(node);
		node = NULL;
	}
}
