#include "minishell.h"

void	sigint_handler_main(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	disable_ctrl_c_main(void)
{
	struct sigaction	ignore_sa;

	ignore_sa.sa_handler = SIG_IGN; // Set to ignore the signal
	sigemptyset(&ignore_sa.sa_mask);
	ignore_sa.sa_flags = 0;
	sigaction(SIGINT, &ignore_sa, NULL);
}

void	sigint_handler_child(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	// printf("\n");
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