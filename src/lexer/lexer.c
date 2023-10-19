#include "minishell.h"

t_lexeme	*create_lexer_array(size_t token_count)
{
	t_lexeme	*lexeme_arr;

	lexeme_arr = malloc(sizeof(t_lexeme) * (token_count + 1));
	if (!lexeme_arr)
	{
		printf("Error: malloc lexeme_arr failed\n");
		return (NULL);
	}
	ft_memset(lexeme_arr, 0, sizeof(t_lexeme) * (token_count + 1));
	return (lexeme_arr);
}

void	command_and_args(size_t token_count, t_lexeme *lexeme_arr)
{
	size_t	i;
	int		command_flag;

	// 0 means we haven't encountered a command yet, 1 means we have
	// TODO: write a macro for that
	i = 0;
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
				lexeme_arr[i].type = L_ARGUMENT;
		}
		else if (lexeme_arr[i].type == L_PIPE)
			command_flag = 0;
		i++;
	}
}

// TODO: operators check before coming here
// If two operators are next to each other, it's an error
t_lexeme	*lexer(t_token *token_arr, t_lexeme *lexeme_arr, char **envp,
		size_t token_count)
{
	size_t	i;

	i = 0;
	while (i < token_count)
	{
		if (token_arr[i].type == T_ENV_VAR)
			lexeme_arr[i] = t_env_var_subs(&token_arr[i], envp);
		else if (token_arr[i].type == T_DOUBLE_QUOTE)
			lexeme_arr[i] = t_double_quotes_var_subs(&token_arr[i], envp);
		else if (token_arr[i].type == T_SINGLE_QUOTE)
			lexeme_arr[i] = single_quote_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_PIPE)
			lexeme_arr[i] = pipe_lexeme(&token_arr[i]);
		else if (token_arr[i].type == T_REDIRECT_IN)
			redirect_in_wrapper(lexeme_arr, token_arr, &i, token_count);
		else if (token_arr[i].type == T_REDIRECT_OUT)
			redirect_out_wrapper(lexeme_arr, token_arr, &i, token_count);
		else if (token_arr[i].type == T_REDIRECT_APPEND)
			redirect_append_wrapper(lexeme_arr, token_arr, &i, token_count);
		else if (token_arr[i].type == T_HEREDOC)
			heredoc_wrapper(lexeme_arr, token_arr, &i);
		else if (token_arr[i].type == T_WORD)
			undefined_wrapper(lexeme_arr, token_arr, &i);
		else
			continue ;
		i++;
	}
	command_and_args(token_count, lexeme_arr);
	return (lexeme_arr);
}

void	lexemize(size_t *token_count, t_token **token_arr,
		t_lexeme **lexeme_arr, char **envp)
{
	*lexeme_arr = create_lexer_array(*token_count);
	*lexeme_arr = lexer(*token_arr, *lexeme_arr, envp, *token_count);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_LEXER)
	{
		printf("\n***Printing lexemes***\n\n");
		print_lexeme_arr(*lexeme_arr, *token_count);
	}
}
