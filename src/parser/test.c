
t_ast_node	*parser(t_lexeme *lexemes, int start, int end, t_data *data)
{
	t_parser vars;
	bool enclosed_cmd;

	enclosed_cmd = is_command_entirely_enclosed(lexemes, start, end);
	init_parser_vars(&vars, start, end);
	while (vars.i >= vars.start)
	{
		printf("lexemes[vars.i].type: %d\n", lexemes[vars.i].type);
		if (lexemes[vars.i].type == L_PARENTHESIS_CLOSED && enclosed_cmd)
		{
			vars.node = parser_parentheses(lexemes, start, vars.i, &vars, data);
			if (vars.node)
				return (vars.node);
		}
		if (lexemes[vars.i].type == L_PARENTHESIS_CLOSED && !enclosed_cmd)
			vars.i = find_parenthesis_sibling(lexemes, start, vars.i);
		if (lexemes[vars.i].type == L_LOG_AND
			|| lexemes[vars.i].type == L_LOG_OR)
		{
			vars.node = parser_log_and_or(lexemes, vars.i, end, &vars, data);
			return (vars.node);
		}
		vars.i--;
	}
	vars.i = end;
	while (vars.i >= vars.start)
	{
		if (lexemes[vars.i].type == L_PIPE)
		{
			vars.node = parser_pipe(lexemes, vars.i, end, &vars, data);
			return (vars.node);
		}
		vars.i--;
	}
	vars.node = build_cmd_node(lexemes, start, end, data);
	return (vars.node);
}