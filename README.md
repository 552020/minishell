# Minishell

## Functions

- `access`: check accessibility of a file.

- `readline`: Reads a line from the terminal and returns it.

- `url_clear_history`: Clears the readline history.

- `rl_on_new_line`: Tells readline that the cursor is on a new line.

- `rl_replace_line`: Replaces the contents of the current input line.

- `rl_redisplay`: Redraws the current input line.

- `add_historya`: Adds a line to the readline history.

- `access`: Checks if the calling process can access the file at a given path.

- `isatty`: Check if a file descriptor is a valid terminal device

Functions:

- *well known*: printf, malloc, free, write, open, read, close, exit
- *known*: access, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sidaddset, kill, execve, dup, dup2, pipe, strerror, perror
- *new*: readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_display, add_history, getcwd, chdir, stat, lstat, fstat, unlink, opendir, readdir, closedir, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, thetflag, tgetnum, tgetstr, tgoto, tupts

## Functions 

## access

- access, faccessat – check accessibility of a file
- <unistd.h>
- `int access(const char *path, int mode);`
- The second argument 'mode' can be R_OK (read), W_OK (write), X_OK (execute/search), F_OK (existence)
- It returns 0 if successful, -1 if not and sets errno.

## stat family

- stat, lstat, fstat


## readline

We use this function to display the prompt to the user and collect the input. 

# tty-functions

TTY is a short form for TeleTYpewriter: https://en.wikipedia.org/wiki/Teleprinter. The tty-functions are a bunch of functions from <unistd.h> which allows to retrieve information about the terminal.

## isatty

`int isatty(int fd);` 

`isatty` takes a file descriptor as argument and returns 1 if the associated file descriptor is a terminal, and 0 if it's not. Uses: if we want to write something to the terminal, or read something from the terminal, we can check if stdin and stdout are associated with the terminal or have been redirected. 

## ttyname

`char *ttyname(int fd);`

It returns a pointer to a string which is the pathname of the terminal associated with the file descriptor fd. If fd is not associated with a terminal, the function returns NULL.

I can't imagine a scenario in which we necessarly needs to use it. It would be useful if we would use multipe terminals, to select the right terminal, but since to ask for the path of the terminal we need to pass the fd of a terminal, i.e. we need to already have that fd, proabably for minishell is not that useful.

## ttyslot

`int ttyslot();`

ttyslot - find the slot of the current user's terminal in some file

It's an archaic function. There is apparently no man page about it in Ubuntu and in macOS the page of ttyname and isatty with no information about ttyslot is shown. 

The ttyslot() function is designed to return the index of the current terminal in the system's terminal configuration file, which could be /etc/ttys or /etc/inittab in older UNIX systems. The function takes no arguments and returns an integer.

## ioctl

It stands for "input/output control". 
It is a general-purpose interface that is used for configuring or getting the status of device-specific parameters. 
In the context of terminal I\O it is often usde to chage terminal settings that cannot be changed using more specialized functions like 'tcsetattr' or 'tcgetattr'.
https://en.wikipedia.org/wiki/Ioctl

Probably it is used only to exit heredoc when CTRL + C or D are pressed while collecting heredoc. 

## getenv

Probably we can not use it if we use envp to get the environemnt variables. Why getenv is actually better than envp: https://42born2code.slack.com/archives/CN9RHKQHW/p1676033337151939. 


