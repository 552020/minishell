#include "minishell.h"

void	redirect_in_wrapper(t_lexeme *lexeme_arr, t_token *token_arr, size_t *i,
		size_t token_count)
{
	lexeme_arr[*i] = redirect_in_lexeme(&token_arr[*i]);
	if (*i + 1 < token_count)
	{
		lexeme_arr[*i + 1] = redirect_in_target_lexeme(&token_arr[*i + 1]);
		(*i)++;
	}
}

void	redirect_out_wrapper(t_lexeme *lexeme_arr, t_token *token_arr,
		size_t *i, size_t token_count)
{
	lexeme_arr[*i] = redirect_out_lexeme(&token_arr[*i]);
	if (*i < token_count - 1)
	{
		lexeme_arr[*i + 1] = redirect_out_target_lexeme(&token_arr[*i + 1]);
		(*i)++;
	}
}

void	redirect_append_wrapper(t_lexeme *lexeme_arr, t_token *token_arr,
		size_t *i, size_t token_count)
{
	lexeme_arr[*i] = redirect_append_lexeme(&token_arr[*i]);
	if (*i < token_count - 1 && token_arr[*i + 1].type != T_END)
	{
		lexeme_arr[*i + 1] = redirect_out_target_lexeme(&token_arr[*i + 1]);
		(*i)++;
	}
}

void	heredoc_wrapper(t_lexeme *lexeme_arr, t_token *token_arr, size_t *i)
{
	lexeme_arr[*i] = heredoc_lexeme(&token_arr[*i]);
	lexeme_arr[*i + 1] = heredoc_delimiter_lexeme(&token_arr[*i + 1]);
	(*i)++;
}

void	undefined_wrapper(t_lexeme *lexeme_arr, t_token *token_arr, size_t *i)
{
	if (lexeme_arr[*i].type != L_FILENAME_STDIN
		|| lexeme_arr[*i].type != L_FILENAME_STDOUT)
		lexeme_arr[*i] = word_lexeme(&token_arr[*i]);
}
