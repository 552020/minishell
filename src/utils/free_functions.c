#include "minishell.h"

void	free_token_arr(t_token *token_arr)
{
	size_t	i;

	if (token_arr == NULL)
		return ;
	i = 0;
	while (token_arr[i].type != T_END)
	{
		if (token_arr[i].str != NULL)
		{
			free(token_arr[i].str);
			token_arr[i].str = NULL;
		}
		i++;
	}
	free(token_arr);
}

void	free_lexeme_arr(t_lexeme *lexeme_arr, t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->token_count + 1)
	{
		if (lexeme_arr[i].str != NULL)
		{
			// printf("freeing lexeme_arr[%zu].str\n", i);
			// printf("lexeme_arr[%zu].str = %s\n", i, lexeme_arr[i].str);
			// printf("lexeme_arr[%zu].type = %d\n", i, lexeme_arr[i].type);
			free(lexeme_arr[i].str);
			lexeme_arr[i].str = NULL;
		}
		// if (lexeme_arr[i].original != NULL)
		// {
		// 	free(lexeme_arr[i].original);
		// 	lexeme_arr[i].original = NULL;
		// }
		i++;
	}
	if (lexeme_arr)
		free(lexeme_arr);
}

void	free_str_arr_ast(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_ast(t_ast_node *node)
{
	if (node == NULL)
		return ;
	if (node->type == N_COMMAND)
	{
		if (node->cmd)
			free(node->cmd);
		if (node->args)
			free_str_arr_ast(node->args);
		if (node->input_file)
			free(node->input_file);
		if (node->output_file)
			free(node->output_file);
		if (node->heredoc_del)
			free(node->heredoc_del);
	}
	else if (node->type == N_PIPE)
	{
		free_ast(node->children[0]);
		free_ast(node->children[1]);
	}
	free(node);
	node = NULL;
}

void	free_key_value_pair(char **key_value)
{
	int	i;

	i = 0;
	while (key_value[i] != NULL)
	{
		free(key_value[i]);
		i++;
	}
	free(key_value);
}

void	free_data(t_data *data)
{
	if (data->env_table)
		free_hash_table(data->env_table);
	if (data->env_arr)
		free_envp(data->env_arr);
	if (data->ast_root)
		free_ast(data->ast_root);
	if (data->token_arr)
		free_token_arr(data->token_arr);
	if (data->lexeme_arr)
		free_lexeme_arr(data->lexeme_arr, data);
}

void	free_exit(t_data *data, char *error_message)
{
	perror(error_message);
	free_data(data);
	exit(FAILURE);
}
