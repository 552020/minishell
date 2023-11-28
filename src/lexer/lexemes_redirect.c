#include "minishell.h"

t_lexeme	redirect_in_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_INPUT;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_in_target_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDIN;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_OUTPUT;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_target_lexeme(t_token *token, t_data *data)
{
	t_lexeme lexeme;

	lexeme.type = L_FILENAME_STDOUT;
	lexeme.str = ft_strdup(token->str);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}