#include "libft.h"
#include <errno.h>
#include <fcntl.h> // for O_RDONLY etc.
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#ifndef MINISHELL_H
# define MINISHELL_H

# define FAILURE 1
# define SUCCESS 0

/* Debugger */
/* Needs to be high */
/* TODO: Achthung external variable*/

typedef enum e_debug_level
{
	DEBUG_OFF,       // No debugging
	DEBUG_TOKENIZER, // Debug the tokenizer
	DEBUG_LEXER,     // Debug the lexer
	DEBUG_AST,       // Debug the AST
	DEBUG_ALL        // Debug everything
}						t_debug_level;

extern t_debug_level	DEBUG_LEVEL;

/* Error messages*/

# define UNEXPECTED_CHAR_WARNING "Warning: Unexpected character during tokenization."

/* Environment Variables*/

# define TABLE_SIZE 42

/* envp */

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_env_table
{
	t_env_var			*table[TABLE_SIZE];
	int count; // This will keep track of the number of environment variables.
}						t_env_table;

void					initialize_table(t_env_table *env_table, char **envp);
void					env(t_env_var **table);
void					export(t_env_table *env_table, char **args,
							char ***envp);
void					unset(t_env_table *env_table, char **args,
							char ***envp);
char					**convert_hash_table_to_array(t_env_table *env_table);
char					*ft_getenv(t_env_var **table, const char *key);

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
	T_DOUBLE_QUOTE,
	// 7 - " the whole string in between " quotes included
	T_SINGLE_QUOTE,
	// 8 - ' the whole string in between ' quotes included
	T_ENV_VAR, // 9 - $ followed by a valid variable name
	T_END,     // 10 - End of token array
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*str;
}						t_token;

int						ft_isspace(int c);
int						isspecialchar(char c);
int						isregularchar(char c, const char *str);
void					skip_spaces(const char **str_ptr);
void					count_word_special_char(const char **str_ptr,
							size_t *words);
size_t					count_words_tokenizer(const char *str);
t_token					*create_token_array(size_t token_count);
void					assign_redirect_in_heredoc(const char **str_ptr,
							t_token *token_arr, size_t *idx);
void					assign_redirect_out_append(const char **str_ptr,
							t_token *token_arr, size_t *idx);
void					assign_redirect_in_out_heredoc_append(const char **str_ptr,
							t_token *token_arr, size_t *idx);
void					handle_unexpected_char(const char **str_ptr);
void					assign_word(const char **str_ptr, t_token *token_arr,
							size_t *idx);
void					assign_pipe(const char **str_ptr, t_token *token_arr,
							size_t *idx);
void					assign_env_var(const char **str_ptr, t_token *token_arr,
							size_t *idx);
void					assign_quotes(const char **str_ptr, t_token *token_arr,
							size_t *idx);
void					tokenize(size_t *token_count, t_token **token_arr,
							char *input);

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

t_lexeme				word_lexeme(t_token *token);
t_lexeme				pipe_lexeme(t_token *token);
t_lexeme				redirect_in_lexeme(t_token *token);
t_lexeme				redirect_out_lexeme(t_token *token);
t_lexeme				redirect_in_target_lexeme(t_token *token);
t_lexeme				redirect_out_target_lexeme(t_token *token);
t_lexeme				redirect_append_lexeme(t_token *token);
t_lexeme				heredoc_lexeme(t_token *token);
t_lexeme				heredoc_delimiter_lexeme(t_token *token);
t_lexeme				t_double_quotes_var_subs(t_token *token, char **envp);
t_lexeme				single_quote_lexeme(t_token *token);
t_lexeme				t_env_var_subs(t_token *token, char **envp);
char					*lookup_env_value(char *var_name, char **envp);
t_lexeme				*create_lexer_array(size_t token_count);
t_lexeme				*lexer(t_token *token_arr, t_lexeme *lexeme_arr,
							char **envp, size_t token_count);
void					redirect_in_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i, size_t token_count);
void					redirect_out_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i, size_t token_count);
void					redirect_append_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i, size_t token_count);
void					heredoc_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i);
void					undefined_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i);
/* Parser */

typedef enum e_node_type
{
	N_PIPE,
	N_COMMAND,
}						t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char *cmd;                      // Data: command, filename
	char **args;                    // Arguments: command arguments
	char *input_file;               // For input redirection.
	char *output_file;              // For output redirection.
	bool append;                    // For output redirection.
	bool heredoc;                   // For heredoc redirection.
	char *heredoc_del;              // For heredoc redirection.
	struct s_ast_node *children[2]; // For output redirection.
}						t_ast_node;

typedef struct s_node_list
{
	t_ast_node			*node;
	struct s_node_list	*next;
}						t_node_list;

t_ast_node				*parser(t_lexeme *lexemes, int start, int end);
t_ast_node				*build_cmd_node(t_lexeme *lexemes, int start, int end);
t_ast_node				*create_node(t_node_type type);
void					append_first_arg(t_ast_node *node, char *arg);
void					append_other_args(t_ast_node *node, char *arg);
void					append_arg_to_command_node(t_ast_node *node, char *arg);
void					handle_cmd_and_args(t_lexeme *lexemes, int idx,
							t_ast_node **node);
void					handle_simple_redirects(t_lexeme *lexemes, int idx,
							t_ast_node **node);
void					handle_double_redirects(t_lexeme *lexemes, int idx,
							t_ast_node **node);
void					print_and_exit(char *str);
void					parse(t_ast_node **ast_root, t_lexeme *lexemes,
							size_t token_count);

/* Varia */
void					check_input(int argc, char **argv);
char					*read_input(void);
t_token					*tokenizer(t_token *token_arr, const char *input);
size_t					count_words_tokenizer(const char *input);

int						ft_isvalidvarname(char c);
void					collect_heredoc_content(t_token *token_arr,
							size_t token_count);
/* Debug */
void					print_token_arr(t_token *token_arr, size_t token_count);
void					print_lexeme_arr(t_lexeme *lexeme_arr,
							size_t lexeme_count);
void					print_ast(t_ast_node *node, int depth);
void					print_ast_new(t_ast_node *node);
void					debug_ast(t_ast_node *node);
t_ast_node				*create_node(t_node_type type);
t_ast_node				*build_cmd_node(t_lexeme *lexemes, int start, int end);

/* Execution */

size_t					count_pipes(t_lexeme *lexeme_arr, size_t token_count);
// not using these
unsigned int			hash(const char *key);
// not using these
void					handle_without_pipes(t_ast_node *ast_root,
							char *dir_paths, char **envp,
							t_env_table *env_table);
void					handle_pipes(t_ast_node *ast_root, char *dir_paths,
							char **envp, t_env_table *env_table);
void					handle_redirections(t_ast_node *node);
void					handle_heredocs(t_ast_node *node);
void					ft_heredoc(t_ast_node *node, char *delimiter);

void					execute(t_ast_node *node, char *dir_paths, char **envp,
							t_env_table *env_table);
void					print_working_directory(void);
void					insert_node_ht(t_env_var **table, const char *key,
							const char *value);
int						lexemize(size_t *token_count, t_token **token_arr,
							t_lexeme **lexeme_arr, char **envp);

#endif
