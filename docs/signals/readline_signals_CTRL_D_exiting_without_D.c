#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// compile with -lreadline: cc readline_signals -lreadline

// In this version when CTRL
//+ D is pressed and the line is empty the program exit but '^D' is printerd by
// the terminal itself not by readline.

void suppress_echoctl() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~(ECHOCTL);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void suppress_ctrl_d_output() {
  char newline = '\n';
  ioctl(STDIN_FILENO, TIOCSTI, &newline);
}

void restore_echoctl() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= ECHOCTL;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

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
  //   struct termios term;
  //   struct termios orig_term;

  if (argc != 1) {
    printf("Usage: %s\n", argv[0]);
    return (1);
  }
  // Get current terminal settings
  //   tcgetattr(0, &orig_term);
  //   term = orig_term;
  // Turn off ECHOCTL
  //   term.c_lflag &= ~(ECHOCTL);
  //   tcsetattr(STDIN_FILENO, TCSANOW, &term);
  signal(SIGINT, handle_sigint);
  signal(SIGQUIT, handle_sigquit);
  while (1) {
    // suppress_echoctl();
    input = readline("$> ");
    // restore_echoctl();
    if (!input) {
      //   suppress_ctrl_d_output();
      printf("exit\n");
      break;
    }
    if (*input)
      add_history(input);
    printf("Input is: %s\n", input);
    free(input);
  }
  // Restore original terminal settings before exit
  //   tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
  return (0);
}
