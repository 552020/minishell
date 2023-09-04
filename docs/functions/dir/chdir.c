#include <stdio.h>
#include <unistd.h>

int main()
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("Current working directory: %s\n", cwd);
	}
	else
	{
		perror("getcwd() error");
		return (1);
	}

	// Change the current working directory to "/tmp"
	if (chdir("..") == 0)
	{
		// Print the new current working directory
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("New working directory: %s\n", cwd);
		}
		else
		{
			perror("getcwd() error");
			return (1);
		}
	}
	else
	{
		perror("chdir() error");
		return (1);
	}
	return (0);
}
