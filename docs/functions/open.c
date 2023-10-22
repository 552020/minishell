#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	ssize_t bytes_read;
	char buffer[1024];

	fd = open("monologue.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening the file\n");
		return (1);
	}

	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}

	if (bytes_read == -1)
	{
		perror("Error reading from the file\n");
		close(fd);
		return (2);
	}
	close(fd);
	return (0);
}
