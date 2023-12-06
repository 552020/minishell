/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 23:35:54 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/29 19:43:38 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	infile_redirection(t_ast_node *node, t_data *data)
{
	int	filein;
	int	i;

	(void)data;
	i = 0;
	while (node->input_files[i])
	{
		filein = 0;
		if (access(node->input_files[i], F_OK) != 0)
		{
			perror(" ");
			if (!node->input_files[i + 1])
				return (FAILURE);
		}
		if (access(node->input_files[i], R_OK) != 0)
		{
			perror(" ");
			if (!node->input_files[i + 1])
				return (FAILURE);
		}
		filein = open(node->input_files[i], O_RDONLY, 0777);
		if (filein == -1)
		{
			perror(" ");
			return (FAILURE);
		}
		i++;
		if (node->input_files[i])
			close(filein);
	}
	dup2(filein, STDIN_FILENO);
	close(filein);
	return (SUCCESS);
}

int	outfile_redirection(t_ast_node *node, t_data *data)
{
	int	fileout;
	int	i;

	(void)data;
	i = 0;
	while (node->output_files[i])
	{
		fileout = 1;
		if (access(node->output_files[i], F_OK) != 0)
		{
			perror(" ");
			if (!node->output_files[i + 1])
				return (FAILURE);
		}
		if (access(node->output_files[i], W_OK) != 0)
		{
			perror(" ");
			if (!node->output_files[i + 1])
				return (FAILURE);
		}
		if (node->append)
			fileout = open(node->output_files[i],
				O_WRONLY | O_CREAT | O_APPEND);
		else if (!node->append)
			fileout = open(node->output_files[i], O_WRONLY | O_CREAT | O_TRUNC);
		if (fileout == -1)
		{
			perror(" ");
			return (FAILURE);
		}
		i++;
		if (node->output_files[i])
			close(fileout);
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

	return_infile = 1;
	return_outfile = 1;
	if (node->heredoc)
		heredoc_redirection(node);
	if (node->input_files)
	{
		if (infile_redirection(node, data))
			return_infile = SUCCESS;
		else
			return_infile = FAILURE;
	}
	if (node->output_files)
	{
		if (outfile_redirection(node, data))
			return_outfile = SUCCESS;
		else
			return_outfile = FAILURE;
	}
	return (return_infile && return_outfile);
}
