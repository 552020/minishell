#include "minishell.h"

int	handle_heredocs(t_ast_node *node)
{
	if (node->type == N_PIPE)
	{
		if (node->children[0])
		{
			if (!handle_heredocs(node->children[0]))
				return (FAILURE);
		}
		if (node->children[1])
		{
			if (!handle_heredocs(node->children[1]))
				return (FAILURE);
		}
	}
	if (node->heredoc)
	{
		if (!node->heredoc_del)
			printf("there is heredoc but not heredoc_del\n");
		if (ft_heredoc(node, node->heredoc_del) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	ft_heredoc(t_ast_node *node, char *delimiter)
{
	pid_t	pid;
	int		fd[2];
	char	*line;
	int		status;
	int		termsig;

	line = NULL;
	if (pipe(fd) == -1)
		error_exit();
	pid = fork();
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
				printf("bash: warning: here-document delimited by end-of-file ");
				printf("(wanted %s')\n", delimiter);
				exit(EXIT_FAILURE);
			}
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
	// if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_AST)
	// {
	// 	printf("\n***Printing AST***\n\n");
	// 	print_ast(node, 7);
	// 	printf("\n***Printing AST NEW***\n\n");
	// 	print_ast_new(node);
	// 	printf("\n*** AST nodes content ***\n\n");
	// 	debug_ast(node);
	// }
}
