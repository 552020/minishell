/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:50:07 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/12 19:17:45 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc(t_ast_node *node, char *delimiter, t_data *data);

void	handle_heredocs(t_ast_node *node, t_data *data)
{
	if (node->type == N_PIPE)
	{
		if (node->children[0])
			handle_heredocs(node->children[0], data);
		if (node->children[1])
			handle_heredocs(node->children[1], data);
	}
	if (node->heredoc)
	{
		if (!node->heredoc_del)
		{
			printf("there is heredoc but not heredoc_del\n");
		}
		ft_heredoc(node, node->heredoc_del, data);
	}
}

void	ft_heredoc(t_ast_node *node, char *delimiter, t_data *data)
{
	pid_t	pid;
	int		fd[2];
	char	*line;

	line = NULL;
	if (pipe(fd) == -1)
		free_exit(data, "pipe error");
	pid = fork();
	if (pid == -1)
		free_exit(data, "fork error");
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = readline("heredoc> ");
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& ft_strlen(delimiter) == ft_strlen(line))
			{
				free(line);
				exit(EXIT_SUCCESS);
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
	}
	close(fd[1]);
	waitpid(pid, NULL, 0);
	node->heredoc_fd = fd[0];
	node->heredoc = true;
}
