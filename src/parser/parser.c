/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:15:23 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/16 00:11:53 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data)
{
	if (data->debug_level == DEBUG_ALL || data->debug_level == DEBUG_AST)
		printf("***Parsing***\n\n");
	// data->ast_root = parser(data->lexeme_arr, 0, data->token_count - 1,
	// data);
	data->ast_root = parser(data->lexeme_arr, 0, data->token_count, data);
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

// t_ast_node	*parser_parentheses(t_parser *vars, t_lexeme *lexemes,
// t_data *data)
t_ast_node	*parser_parentheses(t_lexeme *lexemes, int start, int end,
		t_parser *vars, t_data *data)
{
	printf("parser_parentheses\n");
	vars->parenthesis_sibling = find_parenthesis_sibling(lexemes, start, end);
	if (vars->parenthesis_sibling == -1)
		free_exit(data, "Error: parentheses not balanced\n");
	else if (vars->parenthesis_sibling == start)
	{
		vars->node = create_node(N_PARENTHESES, data);
		build_parentheses_node(vars->node, lexemes, start + 1, end - 1, data);
		return (vars->node);
	}
	else if (vars->parenthesis_sibling > vars->start)
	{
		printf("else if (vars->parenthesis_sibling > vars->start)\n");
		printf("vars->parenthesis_sibling: %d\n", vars->parenthesis_sibling);
		// this is the case where the parentheses are not the first lexemA
		// i.e. they will not be the root node
		vars->i = vars->parenthesis_sibling - 1;
		return (NULL);
	}
	return (NULL);
}

// t_ast_node	*parser_pipe(t_parser *vars, t_lexeme *lexemes, t_data *data)
t_ast_node	*parser_pipe(t_lexeme *lexemes, int start, int end, t_parser *vars,
		t_data *data)
{
	int	l_pipe_found;

	l_pipe_found = 0;
	// printf("parser_pipe\n");
	// printf("start: %d\n", start);
	// printf("end: %d\n", end);
	vars->node = create_node(N_PIPE, data);
	// printf("build_cmd_node\n");
	// printf("start: %d\n", start + 1);
	// printf("end: %d\n", end);
	vars->node->children[1] = build_cmd_node(lexemes, start + 1, end, data);
	end = vars->i - 1;
	vars->i = end;
	// while (vars->i >= vars->start && lexemes[vars->i].type != L_PIPE
	while (vars->i >= vars->start && lexemes[vars->i].type != L_LOG_AND
		&& lexemes[vars->i].type != L_LOG_OR)
	{
		vars->i--;
		if (lexemes[vars->i].type == L_PIPE)
			l_pipe_found = 1;
		// printf("vars->i: %d\n", vars->i);
	}
	if (vars->i > vars->start || l_pipe_found)
	{
		start = vars->i + 1;
		// printf("calling parser in parser_pipe\n");
		vars->node->children[0] = parser(lexemes, start, end, data);
	}
	else
	{
		start = vars->i + 1;
		// printf("start: %d\n", start);
		// printf("end: %d\n", end);
		// printf("calling build_cmd_node in parser_pipe\n");
		vars->node->children[0] = build_cmd_node(lexemes, start, end, data);
	}
	return (vars->node);
}

// vars.start is pointing to the first lexeme of the array
// It gets updated when parse is called recursively
// start is pointing to the matching lexeme
t_ast_node	*parser_log_and_or(t_lexeme *lexemes, int start, int end,
		t_parser *vars, t_data *data)
{
	// printf("parser_log_and_or\n");
	// printf("start: %d\n", start);
	// printf("end: %d\n", end);
	if (lexemes[vars->i].type == L_LOG_OR)
		vars->node = create_node(N_LOG_OR, data);
	else
		vars->node = create_node(N_LOG_AND, data);
	vars->node->children[1] = parser(lexemes, start + 1, end, data);
	end = vars->i - 1;
	vars->i = end;
	while (vars->i >= vars->start && (lexemes[vars->i].type != L_LOG_OR
			|| lexemes[vars->i].type != L_LOG_AND))
		vars->i--;
	// if (vars->i > vars->start)
	// start = vars->i;
	start = vars->i;
	vars->node->children[0] = parser(lexemes, start + 1, end, data);
	return (vars->node);
}

// The problem we face in this loop now is that if we find a paranteses
// which doesn't take the whole command, after finding the matching opening one
// we want to check also for pipes

t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	t_parser	vars;
	bool		enclosed_cmd;

	enclosed_cmd = is_command_entirely_enclosed(lexemes, start, end);
	printf("parser\n");
	printf("start: %d\n", start);
	printf("end: %d\n", end);
	init_parser_vars(&vars, start, end);
	printf("before while loop in parser\n");
	printf("lexemes[vars.i].type: %d\n", lexemes[vars.i].type);
	while (vars.i >= vars.start)
	{
		printf("while loop in parser\n");
		printf("vars.i: %d\n", vars.i);
		printf("vars.start: %d\n", vars.start);
		printf("lexemes[vars.i].type: %d\n", lexemes[vars.i].type);
		if (lexemes[vars.i].type == L_PARENTHESIS_CLOSED && enclosed_cmd)
		{
			vars.node = parser_parentheses(lexemes, start, vars.i, &vars, data);
			if (vars.node)
				return (vars.node);
		}
		printf("after if (lexemes[vars.i].type == L_PARENTHESIS_CLOSED)\n");
		printf("vars.i: %d\n", vars.i);
		printf("lexemes[vars.i].type: %d\n", lexemes[vars.i].type);
		// if (lexemes[vars.i].type == L_PARENTHESIS_OPEN)
		// 	continue ;
		// return (NULL);
		if (lexemes[vars.i].type == L_LOG_AND
			|| lexemes[vars.i].type == L_LOG_OR)
		{
			printf("if (lexemes[vars.i].type == L_LOG_AND ...\n");
			vars.node = parser_log_and_or(lexemes, vars.i, end, &vars, data);
			return (vars.node);
		}
		vars.i--;
	}
	vars.i = end;
	while (vars.i >= vars.start)
	{
		printf("second while loop in parser for L_PIPE\n");
		if (lexemes[vars.i].type == L_PIPE)
		{
			// vars.node = parser_pipe(&vars, lexemes, data);
			vars.node = parser_pipe(lexemes, vars.i, end, &vars, data);
			return (vars.node);
		}
		vars.i--;
	}
	vars.node = build_cmd_node(lexemes, start, end, data);
	return (vars.node);
}
