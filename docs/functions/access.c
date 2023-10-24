#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("No, no, no!\n");
		return (0);
	}
	char *path;
	path = argv[1];

	if(access(path, F_OK) == -1)
	{
		perror("File does not exit or error checking exitence\n")
		return (1);
	}
	if (access(path, R_OK) == 0)
	{
		printf("Read permission is granted.\n");
	}
	else 
	{
		printf("Read permission is denied.\n");
	}

	// Check for write permission
	if(access(path, W_OK) == 0)
	{
		printf("Write permission is granted.\n");
	}
	else
	{
		printf("Write permission is denied.\n");
	}
	return (0);

}
