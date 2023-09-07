#include "minishell.h"
#include <sys/wait.h>


/*
You can start with:

3 - search and launch executables having as input the AST we talked before. 
You can build just a couple of test AST instantiation for testing purpose.


8 - implement redirections: you will get the redirections as 
a node of the AST, the redirection node will be linked to (from) a command node.
 The command not will have a input and an output field, which if not NULL will
  point to the redirection node, which will contain in the data field the name 
  of the infile or outfile


9 - implement pipes: like pipex. The pipe will be also a node, which will have 
two children (leafs), which will be an array of two nodes: the command before 
the pipe as the first element of the pipe and as a second node the second 
command or a pipe node.


14 - implement the following builtins: …
	◦ echo with option -n
	◦ cd with only a relative or absolute path
	◦ pwd with no options
	◦ export with no options
	◦ unset with no options
	◦ env with no options or arguments
	◦ exit with no options
*/

size_t count_pipes(t_lexeme *lexeme_arr, size_t token_count)
{
	size_t i;
	size_t num_of_pipes;

	i = -1;
	num_of_pipes = 0;
	while (++i != token_count + 1)
	{
		if (lexeme_arr[i].type == L_PIPE)
			num_of_pipes++;
	}
	printf("num_of_pipes: %ld\n", num_of_pipes);
	return (num_of_pipes);
}

// int count_pipes(t_ast_node *ast_root)
// {
// 	int i;
// 	int num_of_pipes;

// 	i = 0;
// 	num_of_pipes = 0;
// 	while (&ast_root[i])
// 	{
// 		if (ast_root[i].node->type == N_PIPE)
// 			num_of_pipes++;
// 		i++;
// 	}
// 	printf("num_of_pipes: %d\n", num_of_pipes);
// 	return (num_of_pipes);
// }

void execute_pipeline(t_ast_node *node) {
    if (node == NULL) {
        return;
    }

    if (node->type == N_PIPE) 
	{
        // If the node is a pipe, create a pipe and execute children
        // int pipe_fd[2];
        // if (pipe(pipe_fd) == -1) {
        //     perror("pipe");
        //     exit(EXIT_FAILURE);
        // }

        // Execute the left child with output redirected to the pipe
        // pid_t left_pid = fork();
        // if (left_pid == -1) {
        //     perror("fork");
        //     exit(EXIT_FAILURE);
        // }

        // if (left_pid == 0) {
        //     close(pipe_fd[0]); // Close the read end of the pipe
        //     dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
        //     close(pipe_fd[1]); // Close the write end of the pipe
        //     execute_pipeline(node->children[0]);
        //     exit(EXIT_SUCCESS);
        // }

        // Execute the right child with input from the pipe
        pid_t right_pid = fork();
        if (right_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (right_pid == 0) {
            close(pipe_fd[1]); // Close the write end of the pipe
            dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin from the pipe
            close(pipe_fd[0]); // Close the read end of the pipe
            execute_pipeline(node->children[1]);
            exit(EXIT_SUCCESS);
        }

        // Close both ends of the pipe in the parent
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Wait for both child processes to finish
        waitpid(left_pid, NULL, 0);
        waitpid(right_pid, NULL, 0);
    } else {
        // If the node is not a pipe, execute the command
        execute_command(node);
    }
}

void handle_pipes(t_ast_node *ast_root)
{
	int pipe_fd[2];
    pid_t pid;
	
	// size_t i;

	// i = -1;
	// can add node check if not checked before
	printf("hello");
	// printf("pipe_count: %ld\n", pipe_count);
	printf("ast_root->type: %d\n", ast_root->type);
	if (ast_root->type == N_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1) 
		{
            perror("fork error");
            exit(EXIT_FAILURE);
        }
		if (pid == 0) {
            close(pipe_fd[0]); // Close the read end of the pipe
            dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
            // close(pipe_fd[1]); // Close the write end of the pipe
           	handle_pipes(ast_root->children[0]);
            exit(EXIT_SUCCESS);
        }


	}
}