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

int			ft_heredoc(t_ast_node *node, char *delimiter, t_data *data);

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
	}
	return (SUCCESS);
}
typedef struct s_heredoc
{
	pid_t	pid;
	int		fd[2];
	char	*line;
	int		status;
	int		termsig;
	char	*delimiter;
}			t_heredoc;

void	init_heredoc(t_heredoc *vars)
{
	vars->pid = 0;
	vars->fd[0] = 0;
	vars->fd[1] = 0;
	vars->line = NULL;
	vars->status = 0;
	vars->termsig = 0;
	vars->delimiter = NULL;
}

void	read_heredoc(t_heredoc *vars)
{
	while (1)
	{
		vars->line = readline("heredoc> ");
		if (!vars->line)
		{
			close(vars->fd[1]);
			printf("warning: here-document delimited by end-of-file ");
			printf("(wanted '%s')\n", vars->delimiter);
			exit(EXIT_FAILURE);
		}
		if (ft_strncmp(vars->line, vars->delimiter,
				ft_strlen(vars->delimiter)) == 0
			&& ft_strlen(vars->delimiter) == ft_strlen(vars->line))
		{
			free(vars->line);
			vars->line = NULL;
			exit(EXIT_SUCCESS);
		}
		write(vars->fd[1], vars->line, ft_strlen(vars->line));
		write(vars->fd[1], "\n", 1);
		free(vars->line);
		vars->line = NULL;
	}
}

void	wait_heredoc(t_heredoc *vars)
{
	waitpid(vars->pid, &vars->status, 0);
	if (WIFSIGNALED(vars->status))
	{
		vars->termsig = WTERMSIG(vars->status);
		if (vars->termsig == SIGINT)
		{
			close(vars->fd[0]);
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			exit(EXIT_FAILURE);
		}
	}
}

int	ft_heredoc(t_ast_node *node, char *delimiter, t_data *data)
{
	t_heredoc	vars;

	vars.delimiter = delimiter;
	init_heredoc(&vars);
	if (pipe(vars.fd) == -1)
		free_exit(data, "pipe error");
	vars.pid = fork();
	if (vars.pid == -1)
		free_exit(data, "fork error");
	handle_signals_child(vars.pid);
	if (vars.pid == 0)
	{
		close(vars.fd[0]);
		read_heredoc(&vars);
	}
	close(vars.fd[1]);
	wait_heredoc(&vars);
	node->heredoc_fd = vars.fd[0];
	node->heredoc = true;
	return (SUCCESS);
}
