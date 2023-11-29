#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	create_input_output_files_arr(char ***files_arr, char *new_file,
		t_data *data)
{
	if (new_file == NULL)
		return ;
	*files_arr = (char **)malloc(sizeof(char *) * 2);
	if (*files_arr == NULL)
		free_exit(data, "Error: malloc failed\n");
	*files_arr[0] = ft_strdup(new_file);
	if (*files_arr[0] == NULL)
		free_exit(data, "Error: malloc failed\n");
	*files_arr[1] = NULL;
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
	*files_arr[files_arr_len] = ft_strdup(new_file);
	if (*files_arr[files_arr_len] == NULL)
		free_exit(data, "Error: malloc failed\n");
	*files_arr[files_arr_len + 1] = NULL;
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