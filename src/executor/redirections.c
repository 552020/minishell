/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:35:54 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/08 21:48:22 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	infile_redirection(t_ast_node *node, t_data *data)
{
	int	filein;

	(void)data;
	filein = 0;
	filein = open(node->input_file, O_RDONLY, 0777);
	if (filein == -1)
	{
		perror(" ");
		// free_ast(data->ast_root);
		return (FAILURE);
	}
	dup2(filein, STDIN_FILENO);
	close(filein);
	return (SUCCESS);
}

int	outfile_redirection(t_ast_node *node, t_data *data)
{
	int	fileout;

	(void)data;
	fileout = 1;
	if (node->append)
		fileout = open(node->output_file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (!node->append)
		fileout = open(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
	{
		perror(" ");
		// free_ast(data->ast_root);
		return (FAILURE);
	}
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
	return (SUCCESS);
}

void	heredoc_redirection(t_ast_node *node)
{
	dup2(node->heredoc_fd, STDIN_FILENO);
	close(node->heredoc_fd);
}

int	handle_redirections(t_ast_node *node, t_data *data)
{
	int	return_infile;
	int	return_outfile;

	if (node->heredoc)
		heredoc_redirection(node);
	if (node->input_file)
	{
		if (infile_redirection(node, data))
			return_infile = SUCCESS;
		else
			return_infile = FAILURE;
	}
	// printf("node->output_file: %s\n", node->output_file);
	if (node->output_file)
	{
		if (outfile_redirection(node, data))
			return_outfile = SUCCESS;
		else
			return_outfile = FAILURE;
	}
	return (return_infile && return_outfile);
	// printf("node->output_file: %s\n", node->output_file);
}
