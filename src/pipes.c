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

// Function to execute a single command node
// void execute_command(t_ast_node *node) {
//     pid_t pid;
//     int status;

//     pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) { // Child process
//         // Redirect input if needed
//         if (node->input_file != NULL) {
//             freopen(node->input_file, "r", stdin);
//         }

//         // Redirect output if needed
//         if (node->output_file != NULL) {
//             if (node->append) {
//                 freopen(node->output_file, "a", stdout);
//             } else {
//                 freopen(node->output_file, "w", stdout);
//             }
//         }

//         // Execute the command
//         execvp(node->data, node->args);

//         // If execvp fails, print an error message and exit
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     } else { // Parent process
//         // Wait for the child to finish
//         waitpid(pid, &status, 0);

//         // Check the exit status of the child process
//         if (WIFEXITED(status)) {
//             printf("Child process exited with status %d\n", WEXITSTATUS(status));
//         }
//     }
// }

// void execute_command(t_ast_node *node) {
//     // Redirect input if needed
//     if (node->input_file != NULL) {
//         freopen(node->input_file, "r", stdin);
//     }

//     // Redirect output if needed
//     if (node->output_file != NULL) {
//         if (node->append) {
//             freopen(node->output_file, "a", stdout);
//         } else {
//             freopen(node->output_file, "w", stdout);
//         }
//     }

//     // Execute the command using execvp
//     execvp(node->data, node->args);

//     // If execvp fails, print an error message and exit
//     perror("execvp");
//     exit(EXIT_FAILURE);
// }

void	error_exit(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

char	*path_finder(char *cmd, char *dir_paths)
{
	char	*path;
	char	*path_except_cmd;
	int		i;
    char    **dir_path_arr;

	dir_path_arr = ft_split(dir_paths, ':');
    i = 0;
    while (dir_path_arr[i])
    {
        printf("dir_path_arr[%d]: %s\n", i, dir_path_arr[i]);
        i++;
    }

	i = 0;
	while (dir_path_arr[i])
	{
		path_except_cmd = ft_strjoin(dir_path_arr[i], "/");
		path = ft_strjoin(path_except_cmd, cmd);
		free(path_except_cmd);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (dir_path_arr[i])
		free(dir_path_arr[i++]);
	free(dir_path_arr);
	return (NULL);
}



// void	execute(char *argv, char **envp)
// {
// 	char	**cmd;
// 	char	*path;
// 	int		i;

// 	i = 0;
// 	while (argv[i] != '\'' && argv[i])
// 		i++;
// 	if (argv[i] == '\'' && argv[ft_strlen(argv) - 1] == '\'')
// 		cmd = cmd_with_single_quotes_parser(argv);
// 	else
// 		cmd = ft_split(argv, ' ');
// 	i = 0;
// 	path = path_finder(cmd[0], envp);
// 	if (!path)
// 	{
// 		while (cmd[i])
// 			free(cmd[i++]);
// 		free(cmd);
// 		error_exit();
// 	}
// 	if (execve(path, cmd, envp) == -1)
// 		error_exit();
// }


// change the opem file functions
void execute_command(t_ast_node *node, char *dir_paths, char **envp)
{
    char	*path;
    if (node->input_file != NULL) 
    {
        freopen(node->input_file, "r", stdin);
    }
    if (node->output_file != NULL) 
    {
        if (node->append) 
        {
            freopen(node->output_file, "a", stdout);
        }
        else 
        {
            freopen(node->output_file, "w", stdout);
        }
    }
    path = path_finder(node->data, dir_paths);
    printf("path: %s\n", path);
    if (!path)
	{
            printf("no exec found\n");
		// error_exit();
	}

    printf ("node->data: %s\n", node->data);
    int x;
    // x = 0;
    // while (node->args[x])
    // {
    //     printf ("node->args[%d]: %s\n", x, node->args[x]);
    //     x++;
    // }
    x = 0;

    // Prepare a new array for command and its arguments
    char *cmd_and_args[1024]; // Adjust the array size before func

    // Copy the command into the new array
    cmd_and_args[0] = node->data;

    // Copy the arguments into the new array
    while(node->args[x] != NULL) 
    {
        cmd_and_args[x + 1] = node->args[x];
        x++;
    }
    // Ensure the new array is terminated with a NULL pointer
    cmd_and_args[x + 1] = NULL;
    x = 0;
    while (cmd_and_args[x])
    {
        printf ("cmd_and_args[%d]: %s\n", x, cmd_and_args[x]);
        x++;
    }
    // path is correct,
    // we need to pass arguments including the command so joined data and args
    if (execve(path, cmd_and_args, envp) == -1)
    {
        printf("execve error\n");
  		// error_exit();
    }
}

// cahnge name to ast node
// also remove dir paths and take it from 
// envp or hash table within the function execute_command
void handle_pipes(t_ast_node *ast_root, char *dir_paths,char ** envp)
{
	int pipe_fd[2];
    pid_t left_pid;
    pid_t right_pid;
	
    if (ast_root == NULL)
    {
        return;
	}
    // printf("hello");
	// printf("pipe_count: %ld\n", pipe_count);
	printf("ast_root->type: %d\n", ast_root->type);
	if (ast_root->type == N_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe error\n");
			exit(EXIT_FAILURE);
		}
        // Execute the left child with output redirected to the pipe
		left_pid = fork();
		if (left_pid == -1) 
		{
            perror("fork error");
            exit(EXIT_FAILURE);
        }
		if (left_pid == 0) {
            printf(" left child process created\n");
            close(pipe_fd[0]); // Close the read end of the pipe
            dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
            // close(pipe_fd[1]); // Close the write end of the pipe
           	handle_pipes(ast_root->children[0], dir_paths, envp);
            exit(EXIT_SUCCESS);
        }
        // Execute the right child with input from the pipe
        right_pid = fork();
        if (right_pid == -1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if (right_pid == 0)
        {
            printf(" right child process created\n");
            close(pipe_fd[1]); // Close the write end of the pipe
            dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin from the pipe
            // close(pipe_fd[0]); // Close the read end of the pipe
            handle_pipes(ast_root->children[1], dir_paths, envp);
            exit(EXIT_SUCCESS);
        }
        // Close both ends of the pipe in the parent
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Wait for both child processes to finish
        waitpid(left_pid, NULL, 0);
        waitpid(right_pid, NULL, 0);
	}
    else
    {
        printf("executing command......\n");
        execute_command(ast_root, dir_paths, envp);

    }
}