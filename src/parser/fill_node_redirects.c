/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node_redirects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:10:36 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/30 07:10:38 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_input_output_files_arr(char ***files_arr, char *new_file,
		t_data *data)
{
	if (new_file == NULL)
		return ;
	*files_arr = (char **)malloc(sizeof(char *) * 2);
	if (*files_arr == NULL)
		free_exit(data, "Error: malloc failed\n");
	(*files_arr)[0] = ft_strdup(new_file);
	if (*files_arr[0] == NULL)
		free_exit(data, "Error: malloc failed\n");
	(*files_arr)[1] = NULL;
}

void	update_input_output_files_arr(char ***files_arr, char *new_file,
		t_data *data)
{
	int	files_arr_len;

	files_arr_len = ft_arrlen(*files_arr);
	*files_arr = (char **)ft_realloc(*files_arr, files_arr_len * sizeof(char *),
			(files_arr_len + 2) * sizeof(char *));
	if (*files_arr == NULL)
		free_exit(data, "Error: malloc failed\n");
	(*files_arr)[files_arr_len] = ft_strdup(new_file);
	if ((*files_arr)[files_arr_len] == NULL)
		free_exit(data, "Error: malloc failed\n");
	(*files_arr)[files_arr_len + 1] = NULL;
}

void	create_and_update_input_output_files_arr(char ***files_arr,
		char *new_file, t_data *data)
{
	if (new_file == NULL)
		return ;
	if (*files_arr == NULL)
		create_input_output_files_arr(files_arr, new_file, data);
	else
		update_input_output_files_arr(files_arr, new_file, data);
}

void	handle_simple_redirects(t_lexeme *lexemes, int idx, t_ast_node **node,
		t_data *data)
{
	if (lexemes[idx].type == L_REDIRECT_INPUT)
		;
	else if (lexemes[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (lexemes[idx].type == L_FILENAME_STDIN)
	{
		create_and_update_input_output_files_arr(&(*node)->input_files,
			lexemes[idx].str, data);
	}
	else if (lexemes[idx].type == L_FILENAME_STDOUT)
	{
		create_and_update_input_output_files_arr(&(*node)->output_files,
			lexemes[idx].str, data);
	}
}

void	handle_double_redirects(t_lexeme *lexemes, int idx, t_ast_node **node,
		t_data *data)
{
	if (lexemes[idx].type == L_REDIRECT_INPUT)
		;
	else if (lexemes[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (lexemes[idx].type == L_FILENAME_STDIN)
	{
		create_and_update_input_output_files_arr(&(*node)->input_files,
			lexemes[idx].str, data);
	}
	else if (lexemes[idx].type == L_FILENAME_STDOUT)
	{
		create_and_update_input_output_files_arr(&(*node)->output_files,
			lexemes[idx].str, data);
	}
	else if (lexemes[idx].type == L_HEREDOC_DELIMITER)
	{
		(*node)->heredoc_del = ft_strdup(lexemes[idx].str);
		if ((*node)->heredoc_del == NULL)
			free_exit(data, "Error: malloc failed\n");
		(*node)->heredoc = true;
	}
	else if (lexemes[idx].type == L_REDIRECT_APPEND)
		(*node)->append = true;
}
