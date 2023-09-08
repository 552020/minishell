#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


// CTRL + Z to exit the program. 
void handle_sigint(int sig)
{
	(void)sig;
	printf("Received SIGINT (CTRL+C)\n");
	exit(0);
}

void handle_sigquit(int sig)
{
	(void)sig;
	printf("Received SIGQUIT (CTRL+\\)\n");
	exit(0);
}

int main()
{
	char buffer[10];
	ssize_t n;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	printf("Program started. Press CTRL+C, CTRL+D or CTRL+\\ to send signals.\n");

	while(1)
	{
		n = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (n == 0)
		{
			printf("Received EOF (CTRL+D)\n");
			exit(0);
			}
	}
	return (0);
}
