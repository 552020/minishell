#include "minishell.h"

void	handle_heredocs(t_ast_node *node)
{
	if (node->type == N_PIPE)
	{
		if (node->children[0])
			handle_heredocs(node->children[0]);
		if (node->children[1])
			handle_heredocs(node->children[1]);
	}
	if (node->heredoc)
	{
		if (!node->heredoc_del)
		{
			// check bash for correct statement
			printf("there is heredoc but not heredoc_del\n");
		}
		ft_heredoc(node, node->heredoc_del);
	}
}

void	ft_heredoc(t_ast_node *node, char *delimiter)
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
			// Check if readline returns NULL,
			// which happens on SIGINT
			{
				close(fd[1]);       // Close the write end of the pipe
				exit(EXIT_FAILURE); // Exit with failure status
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
	// waitpid(pid, NULL, 0);
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
			rl_redisplay();
			return ;
		}
	}
	node->heredoc_fd = fd[0];
	node->heredoc = true;
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
