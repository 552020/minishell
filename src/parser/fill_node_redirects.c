#include "minishell.h"

void	handle_simple_redirects(t_lexeme *lexemes, int idx, t_ast_node **node)
{
	if (lexemes[idx].type == L_REDIRECT_INPUT)
		;
	else if (lexemes[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (lexemes[idx].type == L_FILENAME_STDIN)
	{
		if ((*node)->input_file != NULL)
            free((*node)->input_file);
		(*node)->input_file = ft_strdup(lexemes[idx].str);
		if ((*node)->input_file == NULL)
			print_and_exit("Error: malloc node->input_file failed");
	}
	else if (lexemes[idx].type == L_FILENAME_STDOUT)
	{
		if ((*node)->output_file != NULL)
            free((*node)->output_file);
		(*node)->output_file = ft_strdup(lexemes[idx].str);
		if ((*node)->output_file == NULL)
			print_and_exit("Error: malloc node->output_file failed");
	}
}

void	handle_double_redirects(t_lexeme *lexemes, int idx, t_ast_node **node)
{
	if (lexemes[idx].type == L_REDIRECT_INPUT)
		;
	else if (lexemes[idx].type == L_REDIRECT_OUTPUT)
		;
	else if (lexemes[idx].type == L_FILENAME_STDIN)
	{
		if ((*node)->input_file != NULL)
            free((*node)->input_file);
		(*node)->input_file = ft_strdup(lexemes[idx].str);
		if ((*node)->input_file == NULL)
			print_and_exit("Error: malloc node->input_file failed");
	}
	else if (lexemes[idx].type == L_FILENAME_STDOUT)
	{
		if ((*node)->output_file != NULL)
            free((*node)->output_file);
		(*node)->output_file = ft_strdup(lexemes[idx].str);
		if ((*node)->output_file == NULL)
			print_and_exit("Error: malloc node->output_file failed");
	}
	else if (lexemes[idx].type == L_HEREDOC_DELIMITER)
	{
		(*node)->heredoc_del = ft_strdup(lexemes[idx].str);
		if ((*node)->heredoc_del == NULL)
			print_and_exit("Error: malloc node->heredoc_delimiter failed");
		(*node)->heredoc = true;
	}
	else if (lexemes[idx].type == L_REDIRECT_APPEND)
		(*node)->append = true;
}