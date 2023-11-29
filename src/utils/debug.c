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

// for is not allowed!!!
void	print_ast(t_ast_node *node, int depth)
{
	// Print indentation
	for (int i = 0; i < depth; ++i)
		printf("- ");
	// Print node type and cmd
	if (node->type == N_PIPE)
	{
		printf("|\n");
	}
	else if (node->type == N_COMMAND)
	{
		printf("%s", node->cmd);
		if (node->args)
		{
			for (int i = 0; node->args[i] != NULL; ++i)
			{
				printf(" %s", node->args[i]);
			}
		}
		printf("\n");
	}
	// Print children
	if (node->children[0])
		print_ast(node->children[0], depth + 1);
	if (node->children[1])
		print_ast(node->children[1], depth + 1);
}

int	get_max_depth(t_ast_node *node)
{
	int	left_depth;

	if (!node)
		return (0);
	if (node->type == N_COMMAND)
		return (1);
	// If it's a pipe node, we compute the depth of the left child
	left_depth = get_max_depth(node->children[0]);
	// Since it's a pipe, it means there's an additional depth
	left_depth++;
	return (left_depth);
}

void	print_indentation(int depth, bool is_last_sibling[], int last_index)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		if (i == last_index)
		{
			if (is_last_sibling[i])
				printf("    ");
			else
				printf("│   ");
		}
		else
			printf("    ");
		i++;
	}
	if (depth > 0)
	{
		if (is_last_sibling[last_index])
			printf("└── ");
		else
			printf("├── ");
	}
}

void	print_node(t_ast_node *node, int depth, bool is_last_sibling[])
{
	int	i;

	if (!node)
		return ;
	print_indentation(depth, is_last_sibling, depth - 1);
	// Print node type and cmd
	if (node->type == N_PIPE)
	{
		printf("|\n");
	}
	else if (node->type == N_COMMAND)
	{
		printf("%s", node->cmd);
		i = 0;
		while (node->args && node->args[i] != NULL)
		{
			printf(" %s", node->args[i]);
			i++;
		}
		printf("\n");
	}
	// Print children
	if (node->children[0])
	{
		print_node(node->children[0], depth + 1, is_last_sibling);
	}
	if (node->children[1])
	{
		is_last_sibling[depth] = true;
		print_node(node->children[1], depth + 1, is_last_sibling);
	}
}

void	print_ast_new(t_ast_node *root)
{
	bool is_last_sibling[100] = {false};
	// Assuming a max depth of 100; can be dynamically allocated if needed
	print_node(root, 0, is_last_sibling);
}

void	print_node_info(t_ast_node *node)
{
	int	i;

	if (!node)
	{
		printf("Node is NULL\n");
		return ;
	}
	switch (node->type)
	{
	case N_PIPE:
		printf("Type: PIPE\n");
		break ;
	case N_COMMAND:
		printf("Type: COMMAND\n");
		break ;
	default:
		printf("Type: UNKNOWN\n");
		break ;
	}
	// Print data
	if (node->cmd)
	{
		printf("cmd: %s\n", node->cmd);
	}
	else
	{
		printf("Cmd: NULL\n");
	}
	// Print arguments
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
	// if (node->input_file)
	// 	printf("Input Redirection: %s\n", node->input_file);
	// else
	// 	printf("Input Redirection: NULL\n");
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
	// if (node->output_file)
	// {
	// 	printf("Output Redirection: %s\n", node->output_file);
	// 	printf("Append Mode: %s\n", node->append ? "TRUE" : "FALSE");
	// }
	// else
	// 	printf("Output Redirection: NULL\n");
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
		printf("Child 1:\n");
		print_node_info(node->children[0]);
	}
	else
		printf("Child 1: NULL\n");
	if (node->children[1])
	{
		printf("Child 2:\n");
		print_node_info(node->children[1]);
	}
	else
		printf("Child 2: NULL\n");
	printf("-----------\n");
}

void	debug_ast(t_ast_node *root)
{
	printf("------ DEBUGGING AST ------\n");
	print_node_info(root);
	printf("------ END OF AST ------\n");
}
