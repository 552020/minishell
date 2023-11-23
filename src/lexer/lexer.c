#include "minishell.h"

void	create_lexeme_arr(t_data *data)
{
	size_t	i;

	data->lexeme_arr = malloc(sizeof(t_lexeme) * (data->token_count + 1));
	if (!data->lexeme_arr)
		free_exit(data, "Error: malloc lexeme_arr failed\n");
	ft_memset(data->lexeme_arr, 0, sizeof(t_lexeme) * (data->token_count + 1));
	i = 0;
	while (i < data->token_count)
	{
		data->lexeme_arr[i].status = NOT_LEXED;
		data->lexeme_arr[i].type = L_UNDEFINED;
		data->lexeme_arr[i].str = NULL;
		data->lexeme_arr[i].original = NULL;
		i++;
	}
}

void	command_and_args(size_t token_count, t_lexeme *lexeme_arr)
{
	size_t	i;
	int		command_flag;

	i = 0;
	command_flag = NO_CMD_YET;
	while (i < token_count)
	{
		if (lexeme_arr[i].type == L_UNDEFINED)
		{
			if (command_flag == NO_CMD_YET)
			{
				lexeme_arr[i].type = L_COMMAND;
				command_flag = CMD_FOUND;
			}
			else
				lexeme_arr[i].type = L_ARGUMENT;
		}
		else if (lexeme_arr[i].type == L_PIPE)
			command_flag = NO_CMD_YET;
		i++;
	}
}

t_lexeme	*lexer(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->token_count)
	{
		if (data->token_arr[i].type == T_ENV_VAR)
			data->lexeme_arr[i] = t_env_var_subs(&data->token_arr[i], data);
		else if (data->token_arr[i].type == T_DOUBLE_QUOTE)
			data->lexeme_arr[i] = t_double_quotes_var_subs(&data->token_arr[i],
				data);
		else if (data->token_arr[i].type == T_SINGLE_QUOTE)
			data->lexeme_arr[i] = single_quote_lexeme(&data->token_arr[i],
				data);
		else if (data->token_arr[i].type == T_PIPE)
			data->lexeme_arr[i] = pipe_lexeme(&data->token_arr[i], data);
		else if (data->token_arr[i].type == T_REDIRECT_IN)
			redirect_in_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_REDIRECT_OUT)
			redirect_out_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_REDIRECT_APPEND)
			redirect_append_wrapper(&i, data->token_count, data);
		else if (data->token_arr[i].type == T_HEREDOC)
			heredoc_wrapper(data->lexeme_arr, data->token_arr, &i, data);
		else if (data->token_arr[i].type == T_WORD)
			undefined_wrapper(data->lexeme_arr, data->token_arr, &i, data);
		else
			continue ;
		i++;
	}
	data->lexeme_arr[i].type = L_END;
	data->lexeme_arr[i].str = NULL;
	command_and_args(data->token_count, data->lexeme_arr);
	if (data->token_arr[i].type == T_END)
		data->lexeme_arr[i].type = L_END;
	return (data->lexeme_arr);
}

int	lexeme_is_operator(t_lexeme_type type)
{
	if (type == L_PIPE || type == L_REDIRECT_INPUT || type == L_REDIRECT_OUTPUT
		|| type == L_REDIRECT_APPEND || type == L_HEREDOC)
		return (1);
	return (0);
}

int	check_syntax_error(t_lexeme *lexeme_arr)
{
	int	i;

	i = 0;
	while (lexeme_arr[i].type != L_END)
	{
		// printf("lexeme_arr[%d].type = %d\n", i, lexeme_arr[i].type);
		if (lexeme_is_operator(lexeme_arr[i].type))
		{
			if (lexeme_arr[i + 1].type == L_END)
			{
				printf("Syntax error: unexpected end of input\n");
				return (1);
			}
			else if (lexeme_is_operator(lexeme_arr[i + 1].type))
			{
				printf("Syntax error: unexpected token %s\n", lexeme_arr[i
					+ 1].str);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	lexemize(t_data *data)
{
	create_lexeme_arr(data);
	data->lexeme_arr = lexer(data);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_LEXER)
	{
		printf("\n***Lexer***\n\n");
		print_lexeme_arr(data->lexeme_arr, data->token_count);
	}
	free_token_arr(data->token_arr);
	data->token_arr = NULL;
	if (check_syntax_error(data->lexeme_arr))
		return (FAILURE);
	return (SUCCESS);
}
