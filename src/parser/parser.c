/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:15:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/30 07:15:25 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data)
{
	if (data->debug_level == DEBUG_ALL || data->debug_level == DEBUG_AST)
		printf("***Parsing***\n\n");
	data->ast_root = parser(data->lexeme_arr, 0, data->token_count - 1, data);
	if (data->debug_level == DEBUG_ALL || data->debug_level == DEBUG_AST)
	{
		printf("\n***Printing AST***\n\n");
		print_ast(data->ast_root, 7);
		printf("\n***Printing AST NEW***\n\n");
		print_ast_new(data->ast_root);
		printf("\n*** AST nodes content ***\n\n");
		debug_ast(data->ast_root);
	}
	free_lexeme_arr(data);
}

t_ast_node	*parser_parentheses(t_parser *vars, t_lexeme *lexemes, t_data *data)
{
	if (lexemes[vars->i].type == L_PARENTHESIS_CLOSED)
	{
		vars->parenthesis_sibling = find_parenthesis_sibling(lexemes,
			vars->start, vars->end);
		if (vars->parenthesis_sibling == -1)
			free_exit(data, "Error: parentheses not balanced\n");
		else if (vars->parenthesis_sibling == vars->start)
		{
			vars->node = create_node(N_PARENTHESES, data);
			build_parentheses_node(vars->node, lexemes, vars->start + 1,
				vars->end - 1, data);
			return (vars->node);
		}
		else if (vars->parenthesis_sibling > vars->start)
		{
			vars->i = vars->parenthesis_sibling - 1;
		}
	}
	// This sould be never be the case
	if (lexemes[vars->i].type == L_PARENTHESIS_OPEN)
	{
		return (NULL);
	}
	return (NULL);
}

t_ast_node	*parser_pipe(t_parser *vars, t_lexeme *lexemes, t_data *data)
{
	if (lexemes[vars->i].type == L_PIPE)
	{
		vars->node = create_node(N_PIPE, data);
		vars->node->children[1] = build_cmd_node(lexemes, vars->i + 1,
			vars->end, data);
		vars->end = vars->i - 1;
		vars->i = vars->end;
		while (vars->i >= vars->start && lexemes[vars->i].type != L_PIPE)
			vars->i--;
		if (vars->i > vars->start)
			vars->node->children[0] = parser(lexemes, vars->start, vars->end,
				data);
		else
			vars->node->children[0] = build_cmd_node(lexemes, vars->start,
				vars->end, data);
		return (vars->node);
	}
	return (NULL);
}

void	parser_log_and_or(t_parser *vars, t_lexeme *lexemes, t_data *data)
{
	if (lexemes[vars->i].type == L_LOG_AND || lexemes[vars->i].type == L_LOG_OR)
	{
		if (lexemes[vars->i].type == L_LOG_OR)
			vars->node = create_node(N_LOG_OR, data);
		else
			vars->node = create_node(N_LOG_AND, data);
		vars->node->children[1] = parser(lexemes, vars->i + 1, vars->end, data);
		vars->end = vars->i - 1;
		vars->i = vars->end;
		vars->node->children[0] = parser(lexemes, vars->start, vars->end, data);
	}
}

t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	t_parser vars;

	init_parser_vars(&vars, start, end);
	while (vars.i >= vars.start)
	{
		vars.node = parser_parentheses(&vars, lexemes, data);
		if (vars.node)
			return (vars.node);
		vars.node = parser_pipe(&vars, lexemes, data);
		if (vars.node)
			return (vars.node);
		parser_log_and_or(&vars, lexemes, data);
		vars.i--;
	}
	vars.node = build_cmd_node(lexemes, start, end, data);
	return (vars.node);
}