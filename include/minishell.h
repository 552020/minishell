/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 06:20:16 by slombard          #+#    #+#             */
/*   Updated: 2023/12/22 06:43:08 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
// Comment needed to prevent autoformat to move the include above the comment
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>

# define FAILURE 0
# define SUCCESS 1

typedef enum e_debug_level
{
	DEBUG_OFF,
	DEBUG_TOKENIZER,
	DEBUG_LEXER,
	DEBUG_AST,
	DEBUG_ALL
}						t_debug_level;

# define UNEXPECTED_CHAR_WARNING "Unexpected char during tokenization."

# define TABLE_SIZE 42

struct	s_data;

typedef struct s_data	t_data;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_env_table
{
	t_env_var			*table[TABLE_SIZE];
	int					count;
}						t_env_table;

void					initialize_table(char **envp, t_data *data);
int						env(t_env_var **table);

int						export(char **args, t_data *data);
int						unset(char **args, t_data *data);

char					**hash_table_to_arr(t_data *data);
char					*ft_getenv(t_env_var **table, const char *key);

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_REDIRECT_APPEND,
	T_HEREDOC,
	T_HEREDOC_DELIMITER,
	T_DOUBLE_QUOTE,
	T_SINGLE_QUOTE,
	T_ENV_VAR,
	T_SHELL_VAR,
	T_END,
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
char *add_single_or_double_quotes(char *str, char quote);

typedef struct s_var_subs
{
	const char			*str;
	const char			*start;
	const char			*end;
	char				*before;
	char				*var_name;
	char				*before_and_value;
	char				*value;
	char				*after;
	char				*result;
}						t_var_subs;

char					*strip_quotes(char *str, t_data *data);
void					free_var_subs(t_var_subs *vars);
void					free_var_subs_and_exit(t_var_subs *vars, t_data *data,
							char *message);
int						find_next_env_var_if_any(const char **str);

# define NO_CMD_YET 0
# define CMD_FOUND 1
typedef enum	e_lexeme_type
{
	L_COMMAND,
	L_ARGUMENT,
	L_PIPE,
	L_REDIRECT_INPUT,
	L_REDIRECT_OUTPUT,
	L_REDIRECT_APPEND,
	L_HEREDOC,
	L_HEREDOC_DELIMITER,
	L_FILENAME_STDIN,
	L_FILENAME_STDOUT,
	L_UNDEFINED,
	L_END
}						t_lexeme_type;

typedef enum e_lexeme_status
{
	NOT_LEXED = 0,
	LEXED = 1
}						t_lexeme_status;

typedef struct s_lexeme
{
	t_lexeme_type		type;
	char *str;
	char *original;
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
int						check_syntax_error(t_data *data);
void					lexer_t_var_subs(t_data *data, size_t i);
void					lexer_t_quotes_var_subs(t_data *data, size_t i);
void					lexer_t_pipe(t_data *data, size_t i);
void					lexer_t_redirects_and_word(t_data *data, size_t *i);

void					finalize_lexeme_array(t_data *data, size_t i);
void					command_and_args(size_t token_count,
							t_lexeme *lexeme_arr);

typedef enum e_node_type
{
	N_PIPE,
	N_COMMAND,
}						t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char *cmd;
	char **args;
	char **input_files;
	char **output_files;
	bool append;
	int heredoc_fd;
	bool heredoc;
	char *heredoc_del;
	pid_t pid;
	int exit_status;
	int					pipe_id;
	struct s_ast_node *children[2];
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
	t_debug_level		debug_level;
	int					pipes_count;
	int					**pipe_fds;
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

void					check_input(int argc, char **argv);
char					*read_input(t_data *data);
t_token					*tokenizer(t_data *data, const char *input);
size_t					count_words_tokenizer(const char *input);

int						ft_isvalidvarname(char c);
void					collect_heredoc_content(t_token *token_arr,
							size_t token_count);
void					free_ast(t_ast_node *node);
void					free_pipe_fds(int **pipe_fds, int pipes_count);
void					free_hash_table(t_env_table *env_table);
void					free_envp(char **envp);
void					print_token_arr(t_token *token_arr, size_t token_count);
void					print_lexeme_arr(t_lexeme *lexeme_arr,
							size_t lexeme_count);
void					print_ast(t_ast_node *node, int depth);
void					print_ast_new(t_ast_node *node);
void					debug_ast(t_ast_node *node);
void					print_hash_table(t_env_table *env_table);
void					print_envp_arr(char **envp);
t_ast_node				*create_node(t_node_type type, t_data *data);

int						handle_heredocs(t_ast_node *node, t_data *data);

unsigned int			hash(const char *key);
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
char					*ft_realpath(const char *path, t_data *data);
typedef struct s_echo
{
	int					i;
	int					j;
	int					print_newline;
}						t_echo;

void					free_token_arr(t_data *data);
void					free_lexeme_arr(t_data *data);
void					free_key_value_pair(char **key_value);
void					free_ft_exit(t_ast_node *node, char **envp,
							t_env_table *table);
void					free_after_execute(t_data *data);
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
int						handle_single_cmd(t_ast_node *node, t_data *data);
int						handle_l_child(t_ast_node *node, t_data *data,
							pid_t *l_pid, int pipe_fd);
int						handle_r_child(t_ast_node *node, t_data *data,
							pid_t *r_pid, int pipe_fd);
int						signal_status(int status);
void					wait_ast(t_data *data, t_ast_node *node);
void					free_envp(char **envp);
void					free_hash_table(t_env_table *env_table);
size_t					ft_count_word(const char *s, char c);
int						ft_free_ret(char **ret, size_t i);
int						check_parenthesis(t_ast_node *cmd);
void					free_exit_code(t_data *data, char *error_message,
							int exit_code);
void					print_node_info(t_ast_node *node);
void					print_ast_new(t_ast_node *root);
void					handle_execve_fail(t_ast_node *node, t_data *data,
							char *path);
#endif
