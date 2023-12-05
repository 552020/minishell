#include "libft.h"
#include <stdio.h>
// first <stdio.h>
// then the rest of the includes
// if you remove this comments the formatter
// will put the includes in the alphabetical order
// and it will be a mess
#include <errno.h>
#include <fcntl.h> // for O_RDONLY etc.
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#ifndef MINISHELL_H
# define MINISHELL_H

# define FAILURE 0
# define SUCCESS 1

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

extern t_debug_level	g_DEBUG_LEVEL;

/* Error messages*/

# define UNEXPECTED_CHAR_WARNING "Warning: Unexpected character during tokenization."

/* Environment Variables*/

# define TABLE_SIZE 42

/* Forward Declare Free */
struct s_data;

typedef struct s_data	t_data;

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

void					initialize_table(char **envp, t_data *data);
int						env(t_env_var **table);

int						export(char **args, t_data *data);
int						unset(char **args, t_data *data);

char					**hash_table_to_arr(t_data *data);
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
	T_ENV_VAR,   // 9 - $ followed by a valid variable name
	T_SHELL_VAR, // 10 - $ followed by a shell variable symbol like $?
	T_END,       // 11 - End of token array
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*str;
}						t_token;

int						ft_isspace(int c);
int						isspecialchar(char c, const char *str);
int						isregularchar(char c, const char *str);
void					skip_spaces(const char **str_ptr);
void					count_word_special_char(const char **str_ptr,
							size_t *words);
size_t					count_words_tokenizer(const char *str);
t_token					*create_token_array(t_data *data);
void					assign_redirect_in_heredoc(const char **str_ptr,
							t_data *data, size_t *idx);
void					assign_redirect_out_append(const char **str_ptr,
							t_data *data, size_t *idx);
void					assign_redirect_in_out_heredoc_append(const char **str_ptr,
							t_data *data, size_t *idx);
void					handle_unexpected_char(const char **str_ptr);
void					assign_word(const char **str_ptr, t_data *data,
							size_t *idx);
void					assign_pipe(const char **str_ptr, t_data *data,
							size_t *idx);
void					assign_env_var(const char **str_ptr, t_data *data,
							size_t *idx);
void					assign_shell_var(const char **str_ptr, t_data *data,
							size_t *idx);
void					assign_quotes(const char **str_ptr, t_data *data,
							size_t *idx);

void					tokenize(t_data *data, char *input);
char					*reshuffle_single_quotes(const char *input);
char					*reshuffle_double_quotes(const char *input);
char					*find_unpaired_quote(const char *str);
int						count_single_or_double_quotes(const char *str,
							char quote);
char					*remove_single_or_double_quotes(char *str, char quote);
char					*add_single_or_double_quotes(char *str, char quote);

/* Lexer */

# define NO_CMD_YET 0
# define CMD_FOUND 1
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

t_lexeme				word_lexeme(t_token *token, t_data *data);
t_lexeme				pipe_lexeme(t_token *token, t_data *data);
t_lexeme				redirect_in_lexeme(t_token *token, t_data *data);
t_lexeme				redirect_out_lexeme(t_token *token, t_data *data);
t_lexeme				redirect_in_target_lexeme(t_token *token, t_data *data);
t_lexeme				redirect_out_target_lexeme(t_token *token,
							t_data *data);
t_lexeme				redirect_append_lexeme(t_token *token, t_data *data);
t_lexeme				heredoc_lexeme(t_token *token, t_data *data);
t_lexeme				heredoc_delimiter_lexeme(t_token *token, t_data *data);
t_lexeme				t_double_quotes_var_subs(t_token *token, t_data *data);
t_lexeme				single_quote_lexeme(t_token *token, t_data *data);
t_lexeme				t_env_var_subs(t_token *token, t_data *data);
t_lexeme				t_shell_var_subs(t_token *token, t_data *data);
char					*lookup_env_value(char *var_name, char **envp);
void					create_lexeme_arr(t_data *data);
t_lexeme				*lexer(t_data *data);
void					redirect_in_wrapper(size_t *i, size_t token_count,
							t_data *data);
void					redirect_out_wrapper(size_t *i, size_t token_count,
							t_data *data);
void					redirect_append_wrapper(size_t *i, size_t token_count,
							t_data *data);
void					heredoc_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i, t_data *data);
void					undefined_wrapper(t_lexeme *lexeme_arr,
							t_token *token_arr, size_t *i, t_data *data);
char					*process_vars_in_str(const char *str, t_data *data);
char					*strip_quotes(char *str, t_data *data);

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
	char **input_files;             // For input redirection.
	char **output_files;            // For output redirection.
	bool append;                    // For output redirection.
	int heredoc_fd;                 // For heredoc redirection.
	bool heredoc;                   // For heredoc redirection.
	char *heredoc_del;              // For heredoc redirection.
	pid_t pid;                      // Process ID of the command.
	int exit_status;                // Exit status of the command.
	struct s_ast_node *children[2]; // For output redirection.
}						t_ast_node;

typedef struct s_node_list
{
	t_ast_node			*node;
	struct s_node_list	*next;
}						t_node_list;

typedef enum e_ast_type
{
	UNDEFINED,
	SINGLE_CMD_AST,
	NOT_SINGLE_CMD_AST
}						t_ast_type;

typedef struct s_data
{
	t_ast_type			ast_type;
	t_env_table			*env_table;
	char				**env_arr;
	size_t				token_count;
	t_token				*token_arr;
	t_lexeme			*lexeme_arr;
	t_ast_node			*ast_root;
	int					last_exit_status;
}						t_data;

t_ast_node				*parser(t_lexeme *lexemes, int start, int end,
							t_data *data);
t_ast_node				*build_cmd_node(t_lexeme *lexemes, int start, int end,
							t_data *data);
void					append_first_arg(t_ast_node *node, char *arg,
							t_data *data);
void					append_other_args(t_ast_node *node, char *argm,
							t_data *data);
void					append_arg_to_command_node(t_ast_node *node, char *arg,
							t_data *data);
void					handle_cmd_and_args(t_lexeme *lexemes, int idx,
							t_ast_node **node, t_data *data);
void					handle_simple_redirects(t_lexeme *lexemes, int idx,
							t_ast_node **node, t_data *data);
void					handle_double_redirects(t_lexeme *lexemes, int idx,
							t_ast_node **node, t_data *data);
void					print_and_exit(char *str);
void					parse(t_data *data);
void					*ft_realloc(void *ptr, size_t old_size,
							size_t new_size);
int						ft_arrlen(char **arr);

/* Varia */
void					check_input(int argc, char **argv);
char					*read_input(void);
t_token					*tokenizer(t_data *data, const char *input);
size_t					count_words_tokenizer(const char *input);

int						ft_isvalidvarname(char c);
void					collect_heredoc_content(t_token *token_arr,
							size_t token_count);
void					free_ast(t_ast_node *node);
void					free_hash_table(t_env_table *env_table);
void					free_envp(char **envp);
/* Debug */
void					print_token_arr(t_token *token_arr, size_t token_count);
void					print_lexeme_arr(t_lexeme *lexeme_arr,
							size_t lexeme_count);
void					print_ast(t_ast_node *node, int depth);
void					print_ast_new(t_ast_node *node);
void					debug_ast(t_ast_node *node);
void					print_hash_table(t_env_table *env_table);
void					print_envp_arr(char **envp);
t_ast_node				*create_node(t_node_type type, t_data *data);

/* Heredoc */

int						handle_heredocs(t_ast_node *node, t_data *data);

/* Execution */

// not using these
size_t					count_pipes(t_lexeme *lexeme_arr, size_t token_count);
unsigned int			hash(const char *key);
// not using these
// void handle_commands(t_ast_node *ast_root, t_data *data);
int						handle_pipe(t_ast_node *ast_root, t_data *data);
int						handle_redirections(t_ast_node *node, t_data *data);

void					execute_cmd(t_ast_node *node, t_data *data);
int						print_working_directory(void);
int						ft_exit(t_ast_node *node, char **envp,
							t_env_table *table);

void					insert_node_ht(const char *key, const char *value,
							t_data *data);
int						lexemize(t_data *data);
int						change_directory(const char *path);

/* Executor */

void					execute(t_data *data, t_ast_node *node);

void					error_exit(t_ast_node *node, char **envp,
							t_env_table *env_table);
char					*path_finder(char *cmd, char *dir_paths, t_data *data);
void					free_dir_path_arr(char **dir_path_arr);
int						echo(t_ast_node *node);
void					free_cmd_and_args_arr(char **cmd_and_args_arr);

int						builtin_with_args(t_ast_node *node, t_data *data);
int						builtin_without_args(t_ast_node *node, t_data *data);
int						execute_builtin(t_ast_node *node, t_data *data);
int						count_cmd_and_args(t_ast_node *node);
char					**build_cmd_and_args_arr(t_ast_node *node,
							int cmd_and_args_count, t_data *data);
int						command_is_builtin(t_ast_node *node);
void					handle_command_node(t_ast_node *node, char **envp,
							t_env_table *env_table, t_data *data);

/* Builtins*/

typedef struct s_echo
{
	int					i;
	int					j;
	int					print_newline;
}						t_echo;
// void					handle_nodes(t_ast_node *node, char
// *dir_paths, 							char **envp,
// t_env_table *env_table, t_data *data);
void					free_token_arr(t_data *data);
void					free_lexeme_arr(t_data *data);
void					free_key_value_pair(char **key_value);
char					**ft_split_envp(const char *s, char c, t_data *data);
void					free_data(t_data *data);
void					initialize_data(char **envp, t_data *data);
void					free_exit(t_data *data, char *error_message);
void					print_hash_table(t_env_table *env_table);
void					print_envp_arr(char **envp);

void					handle_signals_main(void);
void					handle_signals_child(int pid);
void					disable_ctrl_c_main(void);
char					*trim_ending_trailing_spaces(char const *str);
int						handle_single_command(t_ast_node *node, t_data *data);
int						handle_left_child(t_ast_node *node, t_data *data,
							pid_t *left_pid, int pipe_fd);
int						handle_right_child(t_ast_node *node, t_data *data,
							pid_t *right_pid, int pipe_fd);
int						signal_status(int status);
void					wait_ast(t_data *data, t_ast_node *node);
void					free_envp(char **envp);
void					free_hash_table(t_env_table *env_table);
size_t					ft_count_word(const char *s, char c);
int						ft_free_ret(char **ret, size_t i);
int						check_parenthesis(t_ast_node *cmd);
#endif
