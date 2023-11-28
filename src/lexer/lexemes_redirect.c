#include "minishell.h"

t_token	duplicate_token(t_token *token)
{
	t_token	duplicate;

	duplicate.type = token->type;
	duplicate.str = ft_strdup(token->str);
	if (!duplicate.str)
	{
		printf("Error: malloc duplicate.str failed\n");
		exit(1);
	}
	return (duplicate);
}

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
	t_token		duplicate;

	duplicate = duplicate_token(token);
	lexeme = t_double_quotes_var_subs(&duplicate, data);
	lexeme.type = L_FILENAME_STDIN;
	lexeme.original = NULL;
	if (duplicate.str)
	{
		free(duplicate.str);
		duplicate.str = NULL;
	}
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_lexeme(t_token *token, t_data *data)
{
	t_lexeme	lexeme;

	lexeme = t_double_quotes_var_subs(token, data);
	lexeme.type = L_REDIRECT_OUTPUT;
	lexeme.original = NULL;
	lexeme.status = LEXED;
	return (lexeme);
}

t_lexeme	redirect_out_target_lexeme(t_token *token, t_data *data)
{
	t_lexeme lexeme;
	t_token duplicate;

	duplicate = duplicate_token(token);
	lexeme = t_double_quotes_var_subs(&duplicate, data);
	lexeme.type = L_FILENAME_STDOUT;
	lexeme.original = NULL;
	lexeme.status = LEXED;

	if (duplicate.str)
	{
		free(duplicate.str);
		duplicate.str = NULL;
	}

	return (lexeme);
}