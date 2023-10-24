#include "minishell.h"

void	handle_heredocs(t_ast_node *node)
{
	// if (node->type == N_PIPE)
	// {
	// }
	// if (node->children[1])
	// 	handle_heredocs(node);
	// if (node->children[0])
	// 	handle_heredocs(node);
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
	char	*content;
	char	*line_parent;

	// Prompt the user for input until the heredoc delimiter is entered
	line = NULL;
	if (pipe(fd) == -1)
		error_exit();
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = readline("heredoc> ");
			line = ft_strjoin(line, "\n");
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& ft_strlen(delimiter) == ft_strlen(line) - 1)
			{
				free(line);
				exit(EXIT_SUCCESS);
			}
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
	}
	else
	{
		close(fd[1]);
		// node->arg = read(fd[0]);
		// dup2(fd[0], STDIN_FILENO);
		content = NULL;
		// try same line after
		line_parent = NULL;
		while (1)
		{
			line_parent = readline(NULL);
			line_parent = ft_strjoin(line, "\n");
			content = ft_strjoin(content, line_parent);
			if (ft_strncmp(line_parent, delimiter, ft_strlen(delimiter)) == 0
				&& ft_strlen(delimiter) == ft_strlen(line_parent) - 1)
			{
				free(line_parent);
				break ;
			}
			free(line_parent);
		}
		close(fd[0]); // Close the read end of the pipe
		waitpid(pid, NULL, 0);
		node->args[0] = content;
		node->args[1] = NULL;
		free(content);
	}
	// printf("%s\n", node->args[0]);
	// close(fd[0]);
	// waitpid(pid, NULL, 0);
}

//   else {
//         // In the parent process
//         close(pipe_fd[1]); // Close the write end of the pipe

//
// Read from the read end of the pipe using readline and store the content in node->arg
//         char *content = NULL;
//         char *line;
//         while ((line = readline(NULL)) != NULL) {
//             content = ft_strjoin(content, line);
//             free(line);
//         }
//         close(pipe_fd[0]); // Close the read end of the pipe
//         waitpid(pid, NULL, 0);

//         node->arg = content;
//     }