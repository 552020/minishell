#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Tokenizer */
typedef enum e_token_type
{
	// Maybe change T_WORD to T_CMD_OR_ARG
	// T_WORD could also represent a filename though
	T_WORD,              // Command argument or filename
	T_PIPE,              // |
	T_REDIRECT_IN,       // <
	T_REDIRECT_OUT,      // >
	T_REDIRECT_APPEND,   // >>
	T_HEREDOC,           // <<
	T_HEREDOC_DELIMITER, // << delimiter
	T_HEREDOC_CONTENT,   // << content
	T_DOUBLE_QUOTE,
	// " the whole string in between " quotes included
	T_SINGLE_QUOTE,
	// ' the whole string in between ' quotes included
	T_ENV_VAR, // $ followed by a valid variable name
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*str;
}						t_token;

/* Lexer */
typedef enum e_lexeme_type
{
	L_COMMAND,
	// Command to be executed (could be built-in or external)
	L_ARGUMENT,          // Argument to a command
	L_PIPE,              // Pipe operator, signaling chaining of commands
	L_REDIRECT_INPUT,    // Input redirection operator (<)
	L_REDIRECT_OUTPUT,   // Output redirection operator (>)
	L_REDIRECT_APPEND,   // Append redirection operator (>>)
	L_HEREDOC,           // Heredoc redirection operator (<<)
	L_HEREDOC_DELIMITER, // Delimiter for heredoc (<<)
	L_HEREDOC_CONTENT,   // Content of heredoc (<<)
	L_FILENAME_STDIN,    // Filename used in redirections
	L_FILENAME_STDOUT,   // Filename used in redirections
	L_BUILTIN,           // Built-in shell commands (e.g., echo, cd, pwd)
	L_UNDEFINED          // Undefined lexeme type
}						t_lexeme_type;

typedef enum e_lexeme_status
{
	NOT_LEXED = 0,
	LEXED = 1
}						t_lexeme_status;

typedef struct s_lexeme
{
	t_lexeme_type		type;
	char *str;      // The actual value (could be after variable substitution)
	char *original; // Original value (useful for environment variables)
	t_lexeme_status		status;
}						t_lexeme;

/* Parser */

typedef enum e_node_type
{
	N_PIPE,
	N_COMMAND,
}						t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char *data;                     // Data: command, filename
	char **args;                    // Arguments: command arguments
	char *input_file;               // For input redirection.
	char *output_file;              // For output redirection.
	bool append;                    // For output redirection.
	struct s_ast_node *children[2]; // For output redirection.
}						t_ast_node;

typedef struct s_node_list
{
	t_ast_node			*node;
	struct s_node_list	*next;
}						t_node_list;

t_ast_node				*build_ast(t_lexeme *lexemes, int start, int end);
t_token					*tokenizer(const char *input);
size_t					count_words_tokenizer(const char *input);
t_lexeme				*lexer(t_token *token_arr, char **envp,
							size_t token_count);
int						ft_isvalidvarname(char c);
void					collect_heredoc_content(t_token *token_arr,
							size_t token_count);
