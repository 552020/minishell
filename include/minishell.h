#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> // for O_RDONLY etc.

/* Environment Variables*/

#define TABLE_SIZE 100

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

void					initialize_table(t_env_var **table, char **envp);
void					env(t_env_var **table);
void					export(t_env_var **table, const char *key,
							const char *value);
void					unset(t_env_var **table, const char *key);

/* Tokenizer */
typedef enum e_token_type
{
	// Maybe change T_WORD to T_CMD_OR_ARG
	// T_WORD could also represent a filename though
	T_WORD,              // 0 - Command argument or filename
	T_PIPE,              // 1 - |
	T_REDIRECT_IN,       // 2 - <
	T_REDIRECT_OUT,      // 3 - >
	T_REDIRECT_APPEND,   // 4 - >>
	T_HEREDOC,           // 5 - <<
	T_HEREDOC_DELIMITER, // 6 - << delimiter
	T_HEREDOC_CONTENT,   // 7 - << content
	T_DOUBLE_QUOTE,
	// 8 - " the whole string in between " quotes included
	T_SINGLE_QUOTE,
	// 9 - ' the whole string in between ' quotes included
	T_ENV_VAR, // 10 - $ followed by a valid variable name
	T_END,     // 11 - End of token array
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
	L_UNDEFINED,         // Undefined lexeme type
	L_END                // End of lexeme array
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
	bool append;   	                // For output redirection.
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
/* Debug */
void					print_token_arr(t_token *token_arr, size_t token_count);
void					print_lexeme_arr(t_lexeme *lexeme_arr,
							size_t lexeme_count);
void					print_ast(t_ast_node *node, int depth);

/* Execution */

size_t	count_pipes(t_lexeme *lexeme_arr, size_t token_count); // not using these
unsigned int	hash(const char *key); // not using these
void handle_without_pipes(t_ast_node *ast_root, char *dir_paths,char ** envp);
void handle_pipes(t_ast_node *ast_root, char *dir_paths,char ** envp);
void handle_redirections(t_ast_node *node);
