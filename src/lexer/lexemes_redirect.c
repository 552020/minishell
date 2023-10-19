#include "minishell.h"

t_lexeme	redirect_in_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_INPUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_in_target_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_FILENAME_STDIN;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_lexeme(t_token *token)
{
	t_lexeme	lexeme;

	lexeme.type = L_REDIRECT_OUTPUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_target_lexeme(t_token *token)
{
	t_lexeme lexeme;

	lexeme.type = L_FILENAME_STDOUT;
	lexeme.str = ft_strdup(token->str);
	lexeme.original = ft_strdup(token->str);
	lexeme.status = LEXED;
	return (lexeme);
}