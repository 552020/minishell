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

void	handle_redirections(t_ast_node *node, t_data *data)
{
	if (node->input_file)
		handle_infile(node, data);
	if (node->output_file)
		handle_outfile(node, data);
	if (node->heredoc)
		handle_heredoc(node);
}

void	handle_infile(t_ast_node *node, t_data *data)
{
	int	filein;

	filein = 0;
	filein = open(node->input_file, O_RDONLY, 0777);
	if (filein == -1)
	{
		perror("filein error\n");
		free_ast(data->ast_root);
		return ;
	}
	dup2(filein, STDIN_FILENO);
	close(filein);
}

void	handle_outfile(t_ast_node *node, t_data *data)
{
	int	fileout;

	fileout = 1;
	if (node->append)
		fileout = open(node->output_file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (!node->append)
		fileout = open(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
	{
		perror("fileout error\n");
		free_ast(data->ast_root);
		return ;
	}
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
}

void	handle_heredoc(t_ast_node *node)
{
	dup2(node->heredoc_fd, STDIN_FILENO);
	close(node->heredoc_fd);
}