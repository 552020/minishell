#include <stdio.h>
#include <signal.h>
#include <unistd.h>


// CTRL + Z to exit the program. 
void handle_sigint(int sig)
{
	(void)sig;
	printf("Received SIGINT (CTRL+C)\n");
}

void handle_sigquit(int sig)
{
	(void)sig;
	printf("Received SIGQUIT (CTRL+\\)\n");
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
			printf("Received EOF (CTRL+D)\n");
	}

	return (0);
}
