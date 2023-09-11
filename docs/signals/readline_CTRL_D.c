#include <stdio.h>
//
#include <readline/history.h>
//
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// compile with -lreadline: cc readline_signals -lreadline

void handle_sigint(int sig) {
  (void)sig;
  printf("Received SIGINT (CTRL+C)\n");
  exit(0);
}

void handle_sigquit(int sig) {
  (void)sig;
  printf("Received SIGQUIT (CTRL+\\)\n");
  exit(0);
}

int main(int argc, char **argv) {
  char *input;

  // Configure readline's input function
  rl_getc_function = rl_getc;

  if (argc != 1) {
    printf("Usage: %s\n", argv[0]);
    return (1);
  }
  signal(SIGINT, handle_sigint);
  signal(SIGQUIT, handle_sigquit);
  while (1) {
    input = readline("$> ");
    if (!input) {
      printf("exit\n");
      break;
    }
    if (*input)
      add_history(input);
    printf("Input is: %s\n", input);
    free(input);
  }
  return (0);
}
