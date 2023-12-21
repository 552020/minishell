/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 17:42:28 by slombard          #+#    #+#             */
/*   Updated: 2023/12/18 17:42:31 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token_arr(t_token *token_arr, size_t token_count)
{
	size_t	i;
	char	*token_type;

	i = 0;
	while (i < token_count + 1)
	{
		if (token_arr[i].type == T_WORD)
			token_type = "T_WORD";
		else if (token_arr[i].type == T_PIPE)
			token_type = "T_PIPE";
		else if (token_arr[i].type == T_REDIRECT_IN)
			token_type = "T_REDIRECT_IN";
		else if (token_arr[i].type == T_REDIRECT_OUT)
			token_type = "T_REDIRECT_OUT";
		else if (token_arr[i].type == T_REDIRECT_APPEND)
			token_type = "T_REDIRECT_APPEND";
		else if (token_arr[i].type == T_HEREDOC)
			token_type = "T_HEREDOC";
		else if (token_arr[i].type == T_HEREDOC_DELIMITER)
			token_type = "T_HEREDOC_DELIMITER";
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
			token_type = "T_DOUBLE_QUOTE";
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			token_type = "T_SINGLE_QUOTE";
		else if (token_arr[i].type == T_ENV_VAR)
			token_type = "T_ENV_VAR";
		else if (token_arr[i].type == T_SHELL_VAR)
			token_type = "T_SHELL_VAR";
		else if (token_arr[i].type == T_END)
			token_type = "T_END";
		else
			token_type = "T_UNKNOWN";
		printf("Token %zu: type=%s, str=%s\n", i + 1, token_type,
			token_arr[i].str);
		i++;
	}
}

void	print_lexeme_arr(t_lexeme *lexeme_arr, size_t lexeme_count)
{
	size_t	i;
	char	*lexeme_type;

	i = 0;
	while (i < lexeme_count + 1)
	{
		if (lexeme_arr[i].type == L_COMMAND)
			lexeme_type = "L_COMMAND";
		else if (lexeme_arr[i].type == L_ARGUMENT)
			lexeme_type = "L_ARGUMENT";
		else if (lexeme_arr[i].type == L_PIPE)
			lexeme_type = "L_PIPE";
		else if (lexeme_arr[i].type == L_REDIRECT_INPUT)
			lexeme_type = "L_REDIRECT_INPUT";
		else if (lexeme_arr[i].type == L_REDIRECT_OUTPUT)
			lexeme_type = "L_REDIRECT_OUTPUT";
		else if (lexeme_arr[i].type == L_REDIRECT_APPEND)
			lexeme_type = "L_REDIRECT_APPEND";
		else if (lexeme_arr[i].type == L_HEREDOC)
			lexeme_type = "L_HEREDOC";
		else if (lexeme_arr[i].type == L_HEREDOC_DELIMITER)
			lexeme_type = "L_HEREDOC_DELIMITER";
		else if (lexeme_arr[i].type == L_FILENAME_STDIN)
			lexeme_type = "L_FILENAME_STDIN";
		else if (lexeme_arr[i].type == L_FILENAME_STDOUT)
			lexeme_type = "L_FILENAME_STDOUT";
		else if (lexeme_arr[i].type == L_UNDEFINED)
			lexeme_type = "L_UNDEFINED";
		else if (lexeme_arr[i].type == L_END)
			lexeme_type = "L_END";
		else
			lexeme_type = "L_UNKNOWN";
		printf("Lexeme %zu: type=%s, str=%s\n", i, lexeme_type,
			lexeme_arr[i].str);
		i++;
	}
	printf("\n");
}

void	print_ast(t_ast_node *node, int depth)
{
	for (int i = 0; i < depth; ++i)
		printf("- ");
	if (node->type == N_PIPE)
		printf("|\n");
	else if (node->type == N_COMMAND)
	{
		printf("%s", node->cmd);
		if (node->args)
		{
			for (int i = 0; node->args[i] != NULL; ++i)
				printf(" %s", node->args[i]);
		}
		printf("\n");
	}
	if (node->children[0])
		print_ast(node->children[0], depth + 1);
	if (node->children[1])
		print_ast(node->children[1], depth + 1);
}




void	debug_ast(t_ast_node *root)
{
	printf("------ DEBUGGING AST ------\n");
	print_node_info(root);
	printf("------ END OF AST ------\n");
}

void	print_hash_table(t_env_table *env_table)
{
	int			i;
	t_env_var	*node;

	i = 0;
	while (i < TABLE_SIZE)
	{
		node = env_table->table[i];
		while (node != NULL)
		{
			printf("key: %s\n", node->key);
			printf("value: %s\n", node->value);
			node = node->next;
		}
		i++;
	}
}

void	print_envp_arr(char **envp)
{
	int i;

	printf("print_envp_arr\n");
	i = 0;
	while (envp[i])
	{
		printf("envp[%d]: %s\n", i, envp[i]);
		i++;
	}
}
