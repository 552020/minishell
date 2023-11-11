#include "minishell.h"

void	handle_simple_redirects(int idx, t_ast_node **node, t_data *data)
{
	if (data->lexeme_arr[idx].type == L_REDIRECT_INPUT)
		;
	else if (data->lexeme_arr[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (data->lexeme_arr[idx].type == L_FILENAME_STDIN)
	{
		if ((*node)->input_file != NULL)
            free((*node)->input_file);
		(*node)->input_file = ft_strdup(data->lexeme_arr[idx].str);
		if ((*node)->input_file == NULL)
			free_exit(data, "Error: malloc node->input_file failed\n");
	}
	else if (data->lexeme_arr[idx].type == L_FILENAME_STDOUT)
	{
		if ((*node)->output_file != NULL)
            free((*node)->output_file);
		(*node)->output_file = ft_strdup(data->lexeme_arr[idx].str);
		if ((*node)->output_file == NULL)
			free_exit(data, "Error: malloc node->output_file failed\n");
	}
}

void	handle_double_redirects(int idx, t_ast_node **node, t_data *data)
{
	if (data->lexeme_arr[idx].type == L_REDIRECT_INPUT)
		;
	else if (data->lexeme_arr[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (data->lexeme_arr[idx].type == L_FILENAME_STDIN)
	{
		if ((*node)->input_file != NULL)
            free((*node)->input_file);
		(*node)->input_file = ft_strdup(data->lexeme_arr[idx].str);
		if ((*node)->input_file == NULL)
			free_exit(data, "Error: malloc node->input_file failed\n");
	}
	else if (data->lexeme_arr[idx].type == L_FILENAME_STDOUT)
	{
		if ((*node)->output_file != NULL)
            free((*node)->output_file);
		(*node)->output_file = ft_strdup(data->lexeme_arr[idx].str);
		if ((*node)->output_file == NULL)
			free_exit(data, "Error: malloc node->output_file failed\n");
	}
	else if (data->lexeme_arr[idx].type == L_HEREDOC_DELIMITER)
	{
		(*node)->heredoc_del = ft_strdup(data->lexeme_arr[idx].str);
		if ((*node)->heredoc_del == NULL)
			free_exit(data, "Error: malloc node->heredoc_del failed\n");
		(*node)->heredoc = true;
	}
	else if (data->lexeme_arr[idx].type == L_REDIRECT_APPEND)
		(*node)->append = true;
}