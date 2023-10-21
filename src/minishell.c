#include "minishell.h"

t_debug_level	DEBUG_LEVEL = DEBUG_ALL;

void	handle_heredocs(t_ast_node *node)
{

	if (node->type == N_PIPE)

	else if (node->type == N_COMMAND)
	{
		
	}

	if (node->children[1])
		handle_heredocs(ast_root);
	if (node->children[0])
		handle_heredocs(ast_root);
	if (node->heredoc)
		ft_heredoc(node,node->delimiter);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_table	table;
	char		*input;
	size_t		token_count;
	t_token		*token_arr;
	t_lexeme	*lexeme_arr;
	t_ast_node	*ast_root;
	char		**my_envp;
	char		*my_env_value;

	check_input(argc, argv);
	initialize_table(&table, envp);
	while (1)
	{
		input = read_input();
		tokenize(&token_count, &token_arr, input);
		lexemize(&token_count, &token_arr, &lexeme_arr, envp);
		parse(&ast_root, lexeme_arr, token_count);
		my_envp = convert_hash_table_to_array(&table);
		my_env_value = ft_getenv(table.table, "PATH");
		// heredoc needs to be implemented here, if there is a heredoc, store it as argument of that node
		if (ast_root->type == N_PIPE)
			handle_pipes(ast_root, my_env_value, my_envp);
		else if (ast_root->type == N_COMMAND)
			handle_without_pipes(ast_root, my_env_value, my_envp);
		free(token_arr);
		free(lexeme_arr);
		// TODO: We need to free the AST
		// free(input);
		// Free memory allocated by readline()
	}
	return (0);
}
