#include "minishell.h"

t_lexeme	*lexer(t_token *token_arr, char **envp, size_t token_count)
{
	size_t		i;
	t_lexeme	*lexeme_arr;
	int			command_flag;

	lexeme_arr = malloc(sizeof(t_lexeme) * (token_count + 1));
	if (!lexeme_arr)
	{
		printf("Error: malloc lexeme_arr failed\n");
		return (NULL);
	}
	// TODO: maybe use ft_bzero instead or ft_calloc
	ft_memset(lexeme_arr, 0, sizeof(t_lexeme) * (token_count + 1));
	i = 0;
	while (i < token_count)
	{
		if (token_arr[i].type == T_ENV_VAR)
			lexeme_arr[i] = t_env_var_substitution(&token_arr[i], envp);
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
		{
			lexeme_arr[i] = t_double_quote_var_substitution(&token_arr[i],
				envp);
		}
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			lexeme_arr[i] = single_quote_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_PIPE)
			lexeme_arr[i] = pipe_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_REDIRECT_IN)
		{
			lexeme_arr[i] = redirect_in_lexeme(&token_arr[i]);
			if (i + 1 < token_count)
			{
				lexeme_arr[i + 1] = redirect_in_target_lexeme(&token_arr[i
					+ 1]);
				i++;
			}
		}
		else if (token_arr[i].type == T_REDIRECT_OUT)
		{
			lexeme_arr[i] = redirect_out_lexeme(&token_arr[i]);
			if (i < token_count - 1)
			{
				lexeme_arr[i + 1] = redirect_out_target_lexeme(&token_arr[i
					+ 1]);
				i++;
			}
		}
		else if (token_arr[i].type == T_REDIRECT_APPEND)
		{
			lexeme_arr[i] = redirect_append_lexeme(&token_arr[i]);
			if (lexeme_arr[i + 1].type != L_END && i + 1 < token_count)
				lexeme_arr[i + 1] = redirect_out_target_lexeme(&token_arr[i
					+ 1]);
		}
		else if (token_arr[i].type == T_HEREDOC)
		{
			lexeme_arr[i] = heredoc_lexeme(&token_arr[i]);
			lexeme_arr[i + 1] = heredoc_delimiter_lexeme(&token_arr[i + 1]);
			i++;
		}
		else if (token_arr[i].type == T_WORD)
		{
			if (lexeme_arr[i].type != L_FILENAME_STDIN
				|| lexeme_arr[i].type != L_FILENAME_STDOUT)
				lexeme_arr[i] = word_lexeme(&token_arr[i]);
		}
		else
			continue ;
		i++;
	}
	i = 0;
	i = 0;
	// 0 means we haven't encountered a command yet, 1 means we have
	// TODO: write a macro for that
	command_flag = 0;
	while (i < token_count)
	{
		if (lexeme_arr[i].type == L_UNDEFINED)
		{
			if (command_flag == 0)
			{
				lexeme_arr[i].type = L_COMMAND;
				command_flag = 1;
			}
			else
			{
				lexeme_arr[i].type = L_ARGUMENT;
			}
		}
		else if (lexeme_arr[i].type == L_PIPE)
		{
			command_flag = 0;
			// Reset the command flag when encountering a pipe
		}
		i++;
	}
	// printf("Lexeme array after second round:\n");
	// print_lexeme_arr(lexeme_arr, token_count);
	// printf("\n*Done lexing*\n\n\n");
	return (lexeme_arr);
}
