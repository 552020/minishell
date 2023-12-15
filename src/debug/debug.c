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
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
			token_type = "T_DOUBLE_QUOTE";
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			token_type = "T_SINGLE_QUOTE";
		else if (token_arr[i].type == T_ENV_VAR)
			token_type = "T_ENV_VAR";
		else if (token_arr[i].type == T_LOG_OR)
			token_type = "T_LOG_OR";
		else if (token_arr[i].type == T_LOG_AND)
			token_type = "T_LOG_AND";
		else if (token_arr[i].type == T_PARENTHESIS_OPEN)
			token_type = "T_PARENTHESIS_OPEN";
		else if (token_arr[i].type == T_PARENTHESIS_CLOSED)
			token_type = "T_PARENTHESIS_CLOSED";
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
		else if (lexeme_arr[i].type == L_LOG_OR)
			lexeme_type = "L_LOG_OR";
		else if (lexeme_arr[i].type == L_LOG_AND)
			lexeme_type = "L_LOG_AND";
		else if (lexeme_arr[i].type == L_PARENTHESIS_OPEN)
			lexeme_type = "L_PARENTHESIS_OPEN";
		else if (lexeme_arr[i].type == L_PARENTHESIS_CLOSED)
			lexeme_type = "L_PARENTHESIS_CLOSED";
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
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("- ");
		i++;
	}
	if (node->type == N_PIPE)
		printf("|\n");
	else if (node->type == N_COMMAND)
	{
		printf("%s", node->cmd);
		if (node->args)
		{
			i = 0;
			while (node->args[i] != NULL)
			{
				printf(" %s", node->args[i]);
				i++;
			}
		}
		printf("\n");
	}
	else if (node->type == N_LOG_OR)
		printf("||\n");
	else if (node->type == N_LOG_AND)
		printf("&&\n");
	else
		printf("UNKNOWN\n");
	if (node->children[0])
		print_ast(node->children[0], depth + 1);
	if (node->children[1])
		print_ast(node->children[1], depth + 1);
}

// int	get_max_depth(t_ast_node *node)
// {
// 	int	left_depth;

// 	if (!node)
// 		return (0);
// 	if (node->type == N_COMMAND)
// 		return (1);
// 	left_depth = get_max_depth(node->children[0]);
// 	left_depth++;
// 	return (left_depth);
// }

void	print_node_info(t_ast_node *node, int level)
{
	int	i;

	if (!node)
	{
		printf("Node is NULL\n");
		return ;
	}
	if (node->type == N_PIPE)
		printf("Type: PIPE\n");
	else if (node->type == N_COMMAND)
		printf("Type: COMMAND\n");
	else if (node->type == N_LOG_OR)
		printf("Type: LOG_OR\n");
	else if (node->type == N_LOG_AND)
		printf("Type: LOG_AND\n");
	else if (node->type == N_PARENTHESES)
		printf("Type: PARENTHESIS\n");
	else
		printf("Type: UNKNOWN\n");
	printf("Level: %i\n", level);
	if (node->cmd)
		printf("cmd: %s\n", node->cmd);
	else
		printf("Cmd: NULL\n");
	if (node->args)
	{
		printf("Arguments: ");
		i = 0;
		while (node->args[i] != NULL)
		{
			printf("%s", node->args[i]);
			if (node->args[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
	}
	else
		printf("Arguments: NULL\n");
	if (node->input_files)
	{
		printf("Input Redirections: ");
		i = 0;
		while (node->input_files[i] != NULL)
		{
			printf("%s", node->input_files[i]);
			if (node->input_files[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
	}
	else
		printf("Input Redirections: NULL\n");
	if (node->output_files)
	{
		printf("Output Redirections: ");
		i = 0;
		while (node->output_files[i] != NULL)
		{
			printf("%s", node->output_files[i]);
			if (node->output_files[i + 1] != NULL)
				printf(", ");
			i++;
		}
		printf("\n");
		printf("Append Mode: %s\n", node->append ? "TRUE" : "FALSE");
	}
	else
		printf("Output Redirections: NULL\n");
	if (node->heredoc)
		printf("Heredoc: TRUE\n");
	else
		printf("Heredoc: FALSE\n");
	if (node->heredoc_del)
		printf("Heredoc Delimiter: %s\n", node->heredoc_del);
	else
		printf("Heredoc Delimiter: NULL\n");
	if (node->heredoc_fd != -1)
		printf("Heredoc FD: %d\n", node->heredoc_fd);
	else
		printf("Heredoc FD: -1\n");
	if (node->children[0])
	{
		level = level + 1;
		printf("\n");
		printf("Child 1/%i:\n", level);
		print_node_info(node->children[0], level);
	}
	else
		printf("Child 1: NULL\n");
	if (node->children[1])
	{
		printf("\n");
		printf("Child 2/%i:\n", level);
		print_node_info(node->children[1], level);
	}
	else
		printf("Child 2: NULL\n");
	printf("-----------\n");
}

void	debug_ast(t_ast_node *root)
{
	printf("------ NODES INFO ------\n");
	print_node_info(root, 0);
	printf("------ END OF NODES INFO ------\n");
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