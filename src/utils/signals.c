#include "minishell.h"

void	sigint_handler_main(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_child(int signum)
{
	(void)signum;
	printf("\n");
	exit(0);
}

void	handle_ctrl_c_main(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler_main;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	handle_ctrl_c_child(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_handler_child;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}