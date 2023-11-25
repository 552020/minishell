#include "minishell.h"

void	sigint_handler_main(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
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
	if (sigaction(SIGINT, &ignore_sa, NULL) == -1)
	{
		// to do: handle error
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	handle_signals_main(void)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_handler = sigint_handler_main;
	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
	{
		// to do: handle error
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	if (sigaction(SIGQUIT, &sa_sigquit, NULL) == -1)
	{
		// to do: handle error
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	handle_signals_child(int pid)
{
	struct sigaction sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (pid == 0)
	{
		sa.sa_handler = SIG_DFL;
		if (sigaction(SIGINT, &sa, NULL) == -1)
		{
			// to do: handle error
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
		{
			// to do: handle error
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		sa.sa_handler = SIG_IGN;
		if (sigaction(SIGINT, &sa, NULL) == -1)
		{
			// to do: handle error
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
	}
}