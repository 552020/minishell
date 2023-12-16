/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:57:46 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/30 06:57:50 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_node(t_node_type type, t_data *data)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (new_node == NULL)
		free_exit(data, "Error: malloc node failed\n");
	new_node->type = type;
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->input_files = NULL;
	new_node->output_files = NULL;
	new_node->append = false;
	new_node->heredoc = false;
	new_node->heredoc_fd = -1;
	new_node->heredoc_del = NULL;
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;
	new_node->pid = -1;
	new_node->exit_status = 0;
	return (new_node);
}

t_ast_node	*build_cmd_node(t_lexeme *lexemes, int start, int end, t_data *data)
{
	t_ast_node	*node;
	int			i;

	node = create_node(N_COMMAND, data);
	if (node == NULL)
		free_exit(data, "Error: malloc node failed\n");
	i = start;
	while (i <= end)
	{
		handle_cmd_and_args(lexemes, i, &node, data);
		handle_simple_redirects(lexemes, i, &node, data);
		handle_double_redirects(lexemes, i, &node, data);
		i++;
	}
	return (node);
}

char	**build_args_arr(t_lexeme *lexemes, int start, int end, t_data *data)
{
	char	**args;
	int		i;
	int		j;

	printf("build_args_arr\n");
	printf("start: %d\n", start);
	printf("end: %d\n", end);
	args = (char **)malloc(sizeof(char *) * (end - start + 2));
	if (args == NULL)
		free_exit(data, "Error: malloc args failed\n");
	i = start;
	j = 0;
	while (i <= end)
	{
		if (lexemes[i].type == L_COMMAND || lexemes[i].type == L_ARGUMENT)
			args[j] = ft_strdup(lexemes[i].str);
		else if (lexemes[i].type == L_PIPE)
			args[j] = ft_strdup("|");
		else if (lexemes[i].type == L_REDIRECT_INPUT)
			args[j] = ft_strdup("<");
		else if (lexemes[i].type == L_REDIRECT_OUTPUT)
			args[j] = ft_strdup(">");
		else if (lexemes[i].type == L_REDIRECT_APPEND)
			args[j] = ft_strdup(">>");
		else if (lexemes[i].type == L_HEREDOC)
			args[j] = ft_strdup("<<");
		else if (lexemes[i].type == L_HEREDOC_DELIMITER)
			args[j] = ft_strdup(lexemes[i].str);
		else if (lexemes[i].type == L_FILENAME_STDIN)
			args[j] = ft_strdup(lexemes[i].str);
		else if (lexemes[i].type == L_FILENAME_STDOUT)
			args[j] = ft_strdup(lexemes[i].str);
		else if (lexemes[i].type == L_PARENTHESIS_OPEN)
			args[j] = ft_strdup("(");
		else if (lexemes[i].type == L_PARENTHESIS_CLOSED)
			args[j] = ft_strdup(")");
		else if (lexemes[i].type == L_LOG_AND)
			args[j] = ft_strdup("&&");
		else if (lexemes[i].type == L_LOG_OR)
			args[j] = ft_strdup("||");
		else
		{
			// This should never happen
			args[j] = ft_strdup("ERROR");
			free_exit(data, "Error: Parenthesis node: unknown lexeme type\n");
		}
		if (args[j] == NULL)
			free_exit(data, "Error: ft_strdup failed\n");
		j++;
		i++;
	}
	args[j] = NULL;
	return (args);
}

t_ast_node	*build_parentheses_node(t_ast_node *node, t_lexeme *lexemes,
		int start, int end, t_data *data)
{
	node->cmd = ft_strdup("minishell");
	if (node->cmd == NULL)
		free_exit(data, "Error: ft_strdup failed\n");
	node->args = build_args_arr(lexemes, start, end, data);
	return (node);
}
