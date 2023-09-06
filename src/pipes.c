#include "minishell.h"
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

int count_pipes(t_lexeme *lexeme_arr, size_t token_count)
{
	size_t i;
	size_t num_of_pipes;

	i = token_count;
	num_of_pipes = 0;
	while (i >= 0)
	{
		if (lexeme_arr[i].type == L_PIPE)
			num_of_pipes++;
		i--;
	}
	printf("num_of_pipes: %d\n", num_of_pipes);
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

void executer(t_ast_node *ast_root, size_t pipe_count)
{
	size_t i;

	i = pipe_count;
	while()
}