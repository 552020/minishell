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

typedef struct s_redirections
{
	int	infile;
	int	outfile;
	int	i;
}		t_redirections;

int	open_infile(t_redirections *vars, t_ast_node *node)
{
	if (access(node->input_files[vars->i], F_OK) != 0)
	{
		if (!node->input_files[vars->i + 1])
			return (FAILURE);
	}
	if (access(node->input_files[vars->i], R_OK) != 0)
	{
		if (!node->input_files[vars->i + 1])
			return (FAILURE);
	}
	vars->infile = open(node->input_files[vars->i], O_RDONLY, 0777);
	if (vars->infile == -1)
		return (FAILURE);
	vars->i++;
	if (node->input_files[vars->i])
		close(vars->infile);
	return (SUCCESS);
}

int	open_outfile(t_redirections *vars, t_ast_node *node)
{
	if (access(node->output_files[vars->i], F_OK) != 0)
	{
		if (!node->output_files[vars->i + 1])
			return (FAILURE);
	}
	if (access(node->output_files[vars->i], W_OK) != 0)
	{
		if (!node->output_files[vars->i + 1])
			return (FAILURE);
	}
	if (node->append)
		vars->outfile = open(node->output_files[vars->i],
				O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (!node->append)
		vars->outfile = open(node->output_files[vars->i],
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->outfile == -1)
		return (FAILURE);
	vars->i++;
	if (node->output_files[vars->i])
		close(vars->outfile);
	return (SUCCESS);
}

int	infile_redirection(t_ast_node *node)
{
	t_redirections	vars;

	vars.i = 0;
	vars.infile = 0;
	while (node->input_files[vars.i])
	{
		vars.infile = 0;
		if (!(open_infile(&vars, node)))
			return (FAILURE);
	}
	dup2(vars.infile, STDIN_FILENO);
	close(vars.infile);
	return (SUCCESS);
}

int	outfile_redirection(t_ast_node *node)
{
	t_redirections	vars;

	vars.i = 0;
	vars.outfile = 1;
	while (node->output_files[vars.i])
	{
		vars.outfile = 1;
		if (!(open_outfile(&vars, node)))
			return (FAILURE);
	}
	dup2(vars.outfile, STDOUT_FILENO);
	close(vars.outfile);
	return (SUCCESS);
}

int	handle_redirections(t_ast_node *node, t_data *data)
{
	int	return_infile;
	int	return_outfile;

	(void)data;
	return_infile = 1;
	return_outfile = 1;
	if (node->heredoc)
	{
		dup2(node->heredoc_fd, STDIN_FILENO);
		close(node->heredoc_fd);
	}
	if (node->input_files)
	{
		return_infile = infile_redirection(node);
		// if (return_infile == FAILURE) // Check if infile redirection failed
		// 	return (FAILURE);
	}
	if (node->output_files)
		return_outfile = outfile_redirection(node);
	return (return_infile && return_outfile);
}
