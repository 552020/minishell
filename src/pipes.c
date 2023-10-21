#include "minishell.h"

// not using this for now
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

void	error_exit(void)
{
    // TODO : add free
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
    // while (dir_path_arr[i])
    // {
    //     printf("dir_path_arr[%d]: %s\n", i, dir_path_arr[i]);
    //     i++;
    // }

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



// void	here_doc(char *limiter, int argc)
// {
// 	pid_t	pid;
// 	int		fd[2];
// 	char	*line;

// 	if (argc < 6)
// 		wrong_input();
// 	if (pipe(fd) == -1)
// 		error_exit();
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		line = NULL;
// 		while (line != limiter)
// 		{
// 			line = get_next_line(0);
// 			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && 
// 				ft_strlen(limiter) == ft_strlen(line) - 1)
// 				exit(EXIT_SUCCESS);
// 			write(fd[1], line, ft_strlen(line));
// 		}
// 	}
// 	close(fd[1]);
// 	dup2(fd[0], STDIN_FILENO);
// 	wait(NULL);
// }

void ft_heredoc(char *delimiter)
{
    pid_t	pid;
	int		fd[2];
    // Prompt the user for input until the heredoc delimiter is entered
    char *line;
	if (pipe(fd) == -1)
		error_exit();
    pid = fork();
	if (pid == 0)
    {
		close(fd[0]);
		line = NULL;
		while (1)
		{
			line = readline("heredoc> ");
            line = ft_strjoin(line, "\n");
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 && 
				ft_strlen(delimiter) == ft_strlen(line) - 1)
            {
                free(line);
				exit(EXIT_SUCCESS);
            }
			write(fd[1], line, ft_strlen(line));
            free(line);
		}
	}
    close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	wait(NULL);
}

void handle_redirections(t_ast_node *node)
{
    int filein;
    int fileout;

    // insert heredoc here    try  "<< infile.txt cat"
    filein = 0;
    fileout = 1;
    if (node->input_file)
    {
        printf("Entered input file\n");
        // printf("input file is %s!!!!!!!!!!!!!!!!!!!\n", node->input_file);
            filein = open(node->input_file, O_RDONLY, 0777);
        if (filein == -1)
        {
            // todo : add free and proper exit
            printf("filein error\n");
        }
        dup2(filein, STDIN_FILENO);
        close(filein);
    }
    if (node->heredoc && node->heredoc_del)
        ft_heredoc(node->heredoc_del);
    if (node->output_file != NULL) 
    {
        if (node->append)
            fileout = open(node->output_file, O_WRONLY | O_CREAT | O_APPEND, 0777);
        else if (!node->append)
            fileout = open(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fileout == -1)
        {
            // todo : add free and proper exit
            printf("fileout error\n");
        }
        dup2(fileout, STDOUT_FILENO);
        close(fileout);
    }
}

// change the opem file functions
void execute_command(t_ast_node *node, char *dir_paths, char **envp)
{
    char	*path;
    char **cmd_and_args_arr;
    int cmd_and_args_count;
    int i;
    i = -1;
    
    handle_redirections(node);
    cmd_and_args_count = 0;
    if (node->data)
        while(node->data[++i])
            cmd_and_args_count++;
    i = -1;
    if (node->args)
        while (node->args[++i])
            cmd_and_args_count++;
    path = NULL;
    if (node->data)
        path = path_finder(node->data, dir_paths);
    else
        {
            // in case of there is no command add smtg here
            printf("no commands to execute\n");
        }
    if (!path)
	{
            // TODO : add free if exit
            printf("no exec found\n");
		// error_exit();
	}
    int x;
    x = 0;
    cmd_and_args_arr = (char **)malloc(sizeof(char *) * (cmd_and_args_count + 1));
    // to do : add free
    if(!cmd_and_args_arr)
    {
        // TODO : add free
        printf("malloc error\n");
    }

    // Copy the command into the new array
    if (node->data)
        cmd_and_args_arr[0] = node->data;
    if (node->args != NULL) 
    {
        while (node->args[x] != NULL) 
        {
            cmd_and_args_arr[x + 1] = node->args[x];
            x++;
        }
    }
    cmd_and_args_arr[x + 1] = NULL;
    x = 0;
  
    if (node->data) 
        if (execve(path, cmd_and_args_arr, envp) == -1)
            printf("execve error\n");
}



void handle_without_pipes(t_ast_node *ast_root, char *dir_paths,char ** envp)
{
    pid_t pid;

    pid = fork();
    if (pid == -1) 
	{
            // TODO : add free
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) 
	{
        // printf("executing command......\n");
        execute_command(ast_root, dir_paths, envp);
    }
    waitpid(pid, NULL, 0);
}


// cahnge name to ast node
// also remove dir paths and take it from 
// envp or hash table within the function execute_command
// can't use envp, get all the info from ht
void handle_pipes(t_ast_node *ast_root, char *dir_paths,char ** envp)
{
	int pipe_fd[2];
    pid_t left_pid;
    pid_t right_pid;
	
	// printf("ast_root->type: %d\n", ast_root->type);

	if (ast_root->type == N_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
            // TODO : add free
			perror("pipe error\n");
			exit(EXIT_FAILURE);
		}
        // Execute the left child with output redirected to the pipe
		left_pid = fork();
		if (left_pid == -1) 
		{
            // TODO : add free
            perror("fork error");
            exit(EXIT_FAILURE);
        }
		if (left_pid == 0) 
        {
            // printf(" left child process created\n");
            close(pipe_fd[0]); // Close the read end of the pipe
            dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
            close(pipe_fd[1]); // Close the write end of the pipe

            handle_redirections(ast_root->children[0]);
           	handle_pipes(ast_root->children[0], dir_paths, envp);
            // TODO : add free (maybe)
            exit(EXIT_SUCCESS);
        }
        // Execute the right child with input from the pipe
        right_pid = fork();
        if (right_pid == -1)
        {
            // TODO : add free
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if (right_pid == 0)
        {
            // printf(" right child process created\n");
            close(pipe_fd[1]); // Close the write end of the pipe
            dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin from the pipe
            close(pipe_fd[0]); // Close the read end of the pipe

            handle_redirections(ast_root->children[1]);
            handle_pipes(ast_root->children[1], dir_paths, envp);
            // TODO : add free (maybe)
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
        // create another child process for that case otherwise it will exit
        // printf("executing command......\n");
        execute_command(ast_root, dir_paths, envp);
    }
}

// Notes:
// 1) implement redirections that can be used with and without rl_dump_functions
// ex1:  cat test.txt | >> test.txt (appends)
// ex2:  cat test.txt | grep "hello" >> test.txt (appends)
// ex3:  cat test.txt | grep "hello" >> test.txt | cat test.txt (does not append)
// 2)implement open file and close file for redirections correctly
// Errors:

// 1)
// correct:
// cat infile.txt | grep h
// This is computer
// hello world

// wrong:
// cat infile.txt | grep h
// stuck at lexing first round

// 2) cat infile.txt >> outfile.txt | cat outfile.txt (this sometimes appends sometimes not in shell)
//   our version does not append and I dont know if it should
