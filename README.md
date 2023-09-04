# Minishell

# Functions

- `readline`: Reads a line from the terminal and returns it.

- `url_clear_history`: Clears the readline history.

- `rl_on_new_line`: Tells readline that the cursor is on a new line.

- `rl_replace_line`: Replaces the contents of the current input line.

- `rl_redisplay`: Redraws the current input line.

- `add_historya`: Adds a line to the readline history.

- `printf`: Prints formatted output to stdout.

- `malloc`: Allocates memory.

- `free`: Deallocates memory that was previously allocated by malloc, calloc or realloc.

- `write`: Writes data to a file descriptor.

- `access`: Checks if the calling process can access the file at a given path.

Functions:

- *well known*: printf, malloc, free, write, open, read, close, exit
- *known*: access, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sidaddset, kill, execve, dup, dup2, pipe, strerror, perror
- *new*: readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_display, add_history, getcwd, chdir, stat, lstat, fstat, unlink, opendir, readdir, closedir, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, thetflag, tgetnum, tgetstr, tgoto, tupts

## Functions we already 

## readline

We use this function to display the prompt to the user and collect the input. 
# minishell
