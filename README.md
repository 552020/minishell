# Minishell

# Functions

- `readline`: Reads a line from the terminal and returns it.

- `url_clear_history`: Clears the readline history.

- `rl_on_new_line`: Tells readline that the cursor is on a new line.

- `rl_replace_line`: Replaces the contents of the current input line.

- `rl_redisplay`: Redraws the current input line.

- `add_history`: Adds a line to the readline history.

- `printf`: Prints formatted output to stdout.

- `malloc`: Allocates memory.

- `free`: Deallocates memory that was previously allocated by malloc, calloc or realloc.

- `write`: Writes data to a file descriptor.

- `access`: Checks if the calling process can access the file at a given path.

- `isatty`: Check if a file descriptor is a valid terminal device

Functions:

- _well known_: printf, malloc, free, write, open, read, close, exit
- _known_: access, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sidaddset, kill, execve, dup, dup2, pipe, strerror, perror
- _new_: readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_display, add_history, getcwd, chdir, stat, lstat, fstat, unlink, opendir, readdir, closedir, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, thetflag, tgetnum, tgetstr, tgoto, tupts

## Functions we already

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

## tcsetattr

`tcsetattr(int fd, int optional_actions, const struct termios *termios_p)`

- Used to set terminal attributes (like baud rate, flow control, etc.) for the terminal referred by the fd.
- <termios.h> termios means 'TERMinal Input Output Settings'

Use: it could be use to soppress the CTRL + C sign when interrupting a process with a signal:

- https://42born2code.slack.com/archives/CMX2R5JSW/p1659104268134569
- https://42born2code.slack.com/archives/CN9RHKQHW/p1632837006412900
- https://42born2code.slack.com/archives/CMX2R5JSW/p1632256884432600

The man page group it together with: cfgetispeed, cfgetospeed, cfmakeraw, cfsetispeed, cfsetospeed,
cfsetspeed, tcgetattr, tcsetattr – manipulating the termios structure
