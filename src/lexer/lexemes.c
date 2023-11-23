#include "minishell.h"

t_lexeme	pipe_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_PIPE;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	// lexeme.original = ft_strdup(token->str);
	// if (!lexeme.original)
	// free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	log_or_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_LOG_OR;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	log_and_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_LOG_AND;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	parentheses_open_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_PARENTHESES_OPEN;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}
t_lexeme	parentheses_close_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	(void)token;
	(void)data;
	lexeme.type = L_PARENTHESES_CLOSE;
	lexeme.str = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}
/* To this point we don't have a delimiter token anymore */
t_lexeme	redirect_append_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_APPEND;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	// lexeme.original = ft_strdup(token->str);
	// if (!lexeme.original)
	// free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	heredoc_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_HEREDOC;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	// lexeme.original = ft_strdup(token->str);
	// if (!lexeme.original)
	// free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	heredoc_delimiter_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_HEREDOC_DELIMITER;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	// lexeme.original = ft_strdup(token->str);
	// if (!lexeme.original)
	// free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	word_lexeme(t_token *token, t_data *data)
{
	t_lexeme lexeme;

	lexeme.type = L_UNDEFINED;
	// printf("token->str: %s\n", token->str);
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	// lexeme.original = ft_strdup(token->str);
	// if (!lexeme.original)
	// free_exit(data, "Error: malloc lexeme.original failed\n");
	lexeme.status = NOT_LEXED;
	// printf("lexeme.str: %s\n", lexeme.str);
	return (lexeme);
}