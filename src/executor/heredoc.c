/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:50:07 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/26 04:10:29 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_heredoc(t_ast_node *node, char *delimiter, t_data *data);

int	handle_heredocs(t_ast_node *node, t_data *data)
{
	if (node->type == N_PIPE)
	{
		if (node->children[0])
		{
			if (!handle_heredocs(node->children[0], data))
				return (FAILURE);
		}
		if (node->children[1])
		{
			if (!handle_heredocs(node->children[1], data))
				return (FAILURE);
		}
	}
	if (node->heredoc)
	{
		if (!node->heredoc_del)
		{
			printf("there is heredoc but not heredoc_del\n");
		}
		if (ft_heredoc(node, node->heredoc_del, data) == FAILURE)
			return (FAILURE);
		// ft_heredoc(node, node->heredoc_del, data);
	}
	return (SUCCESS);
}

int	ft_heredoc(t_ast_node *node, char *delimiter, t_data *data)
{
	pid_t	pid;
	int		fd[2];
	char	*line;
	int		status;
	int		termsig;

	line = NULL;
	if (pipe(fd) == -1)
		free_exit(data, "pipe error");
	pid = fork();
	if (pid == -1)
		free_exit(data, "fork error");
	handle_signals_child(pid);
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line)
			{
				close(fd[1]);
				printf("minishell: warning: here-document delimited by end-of-file ");
				printf("(wanted '%s')\n", delimiter);
				exit(EXIT_FAILURE);
			}
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& ft_strlen(delimiter) == ft_strlen(line))
			{
				free(line);
				line = NULL;
				exit(EXIT_SUCCESS);
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
			line = NULL;
		}
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		termsig = WTERMSIG(status);
		if (termsig == SIGINT)
		{
			close(fd[0]);
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			// This was causing the double prompt
			// rl_redisplay();
			return (FAILURE);
		}
	}
	node->heredoc_fd = fd[0];
	node->heredoc = true;
	return (SUCCESS);
}
