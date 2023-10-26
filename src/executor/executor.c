#include "minishell.h"

void	handle_redirections(t_ast_node *node)
{
	int	filein;
	int	fileout;

	filein = 0;
	fileout = 1;
	if (node->input_file)
	{
		filein = open(node->input_file, O_RDONLY, 0777);
		if (filein == -1)
		{
			// todo : add free and proper exit
			printf("filein error\n");
		}
		dup2(filein, STDIN_FILENO);
		close(filein);
	}
	if (node->output_file != NULL)
	{
		if (node->append)
			fileout = open(node->output_file, O_WRONLY | O_CREAT | O_APPEND,
					0777);
		else if (!node->append)
			fileout = open(node->output_file, O_WRONLY | O_CREAT | O_TRUNC,
					0777);
		if (fileout == -1)
		{
			// todo : add free and proper exit
			printf("fileout error\n");
		}
		dup2(fileout, STDOUT_FILENO);
		close(fileout);
	}
	if (node->heredoc)
	{
		dup2(node->heredoc_fd, STDIN_FILENO);
		close(node->heredoc_fd);
	}
}

// change the open file functions
int	count_cmd_and_args(t_ast_node *node)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (node->cmd)
		count++;
	if (node->args)
		while (node->args[i])
		{
			count++;
			i++;
		}
	return (count);
}

char	**build_cmd_and_args_arr(t_ast_node *node, int cmd_and_args_count)
{
	char	**cmd_and_args_arr;
	int		i;

	i = 0;
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = (char **)malloc(sizeof(char *) * (cmd_and_args_count
				+ 1));
	if (!cmd_and_args_arr)
	{
		// TODO : add free
		printf("malloc error\n");
	}
	// Copy the command into the new array
	if (node->cmd)
		cmd_and_args_arr[0] = node->cmd;
	if (node->args != NULL)
	{
		while (node->args[i] != NULL)
		{
			cmd_and_args_arr[i + 1] = node->args[i];
			i++;
		}
	}
	cmd_and_args_arr[i + 1] = NULL;
	return (cmd_and_args_arr);
}

int	command_is_builtin(t_ast_node *node)
{
	if (ft_strncmp(node->cmd, "env", 3) == 0 && ft_strlen(node->cmd) == 3)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "export", 6) == 0
		&& ft_strlen(node->cmd) == 6)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "pwd", 3) == 0 && ft_strlen(node->cmd) == 3)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "unset", 5) == 0
		&& ft_strlen(node->cmd) == 5)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "cd", 2) == 0 && ft_strlen(node->cmd) == 2)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "echo", 4) == 0 && ft_strlen(node->cmd) == 4)
		return (SUCCESS);
	else if (ft_strncmp(node->cmd, "exit", 4) == 0 && ft_strlen(node->cmd) == 4)
		return (SUCCESS);
	else
		return (FAILURE);
}

void	execute_builtin(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	int		cmd_and_args_count;
	char	**cmd_and_args_arr;

	(void)node;
	(void)dir_paths;
	(void)envp;
	(void)env_table;
	(void)cmd_and_args_arr;
	handle_redirections(node);
	cmd_and_args_count = count_cmd_and_args(node);
	cmd_and_args_arr = build_cmd_and_args_arr(node, cmd_and_args_count);
	if (ft_strncmp(node->cmd, "env", 3) == 0 && ft_strlen(node->cmd) == 3)
		env(env_table->table);
	if (ft_strncmp(node->cmd, "export", 6) == 0 && ft_strlen(node->cmd) == 6)
	{
		// TODO: implement ARG="arg" in the lexer
		export(env_table, cmd_and_args_arr, &envp);
	}
	if (ft_strncmp(node->cmd, "unset", 5) == 0 && ft_strlen(node->cmd) == 5)
		unset(env_table, cmd_and_args_arr, &envp);
	if (ft_strncmp(node->cmd, "pwd", 3) == 0 && ft_strlen(node->cmd) == 3)
		print_working_directory();
	if (ft_strncmp(node->cmd, "cd", 2) == 0 && ft_strlen(node->cmd) == 2)
	{
		if (node->args)
			change_directory(node->args[0]);
		else
		{
			printf("Sorry! Cd works only with some args!\n");
		}
	}
	if (ft_strncmp(node->cmd, "echo", 4) == 0 && ft_strlen(node->cmd) == 4)
	{
		echo(node);
	}
	if (ft_strncmp(node->cmd, "exit", 4) == 0 && ft_strlen(node->cmd) == 4)
	{
		ft_exit(0, node, envp, env_table);
	}
}
void	execute_cmd(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	char	*path;
	char	**cmd_and_args_arr;
	int		cmd_and_args_count;

	(void)env_table;
	// printf("before redirections\n");
	handle_redirections(node);
	cmd_and_args_count = count_cmd_and_args(node);
	path = NULL;
	if (node->cmd)
	{
		path = path_finder(node->cmd, dir_paths);
		if (!path)
		{
			// TODO : add free if exit
			printf("no exec found\n");
			// error_exit();
		}
	}
	else
		printf("no commands to execute\n");
	cmd_and_args_arr = build_cmd_and_args_arr(node, cmd_and_args_count);
	if (node->cmd)
	{
		if (execve(path, cmd_and_args_arr, envp) == -1)
			printf("execve error\n");
	}
}

void	handle_without_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	pid_t	pid;

	// printf("hanlding without pipes\n");
	if (command_is_builtin(node))
	{
		execute_builtin(node, dir_paths, envp, env_table);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		// TODO : add free
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// printf("before execute_cmd\n");
		execute_cmd(node, dir_paths, envp, env_table);
	}
	waitpid(pid, NULL, 0);
}

void	handle_pipes(t_ast_node *node, char *dir_paths, char **envp,
		t_env_table *env_table)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	// printf("node->type: %d\n", node->type);
	if (node->type == N_PIPE)
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
			close(pipe_fd[0]);
			// Close the read end of the pipe
			dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
			close(pipe_fd[1]);
			// Close the write end of the pipe
			handle_redirections(node->children[0]);
			handle_pipes(node->children[0], dir_paths, envp, env_table);
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
			close(pipe_fd[1]);
			// Close the write end of the pipe
			dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin from the pipe
			close(pipe_fd[0]);
			// Close the read end of the pipe
			handle_redirections(node->children[1]);
			handle_pipes(node->children[1], dir_paths, envp, env_table);
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
		if (command_is_builtin(node))
		{
			execute_builtin(node, dir_paths, envp, env_table);
		}
		else
			execute_cmd(node, dir_paths, envp, env_table);
	}
}

void	execute(t_ast_node *ast_root, char *dir_paths, char **my_envp,
		t_env_table *env_table)
{
	if (ast_root->type == N_PIPE)
		handle_pipes(ast_root, dir_paths, my_envp, env_table);
	else if (ast_root->type == N_COMMAND)
		handle_without_pipes(ast_root, dir_paths, my_envp, env_table);
}
