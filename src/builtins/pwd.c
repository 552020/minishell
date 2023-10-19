#include "minishell.h"

#define MAX_PATH_SIZE 1024

void	print_working_directory(void)
{
	char buffer[MAX_PATH_SIZE];

	if (getcwd(buffer, sizeof(buffer)) != NULL)
	{
		printf("%s\n", buffer);
	}
	else
	{
		perror("Error getting current working directory");
	}
}