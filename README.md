# Minishell

# Minishell

An implementation of a simple Unix shell in C, crafted as a hands-on project for the School 42 curriculum.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Development](#development)
  - [Dependencies](#dependencies)
  - [Building from Source](#building-from-source)
- [Contributors](#contributors)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Introduction

Minishell` is a simple Unix shell developed in C as part of the School 42 curriculum. This is a collaborative project developed by [Batu](https://www.yourlinkhere.com/batu) and [Stefano](https://www.yourlinkhere.com/stefano). The code strictly adheres to the Norm, which imposes certain (at times questionable) constraints such as a maximum function length of 25 lines, the prohibition of for-loops, and [more](add_internal_link/to_the_norm).

## Features

- [x] Command execution using PATH variable or direct path
- [x] Input redirection (`<`) and output redirection (`>` & `>>`)
- [x] Handling of environment variables (like `$PATH`)
- [x] Signal management (e.g., `ctrl-C`, `ctrl-D`)
- [x] Built-in commands support (`echo`, `cd`, `pwd`, etc.)
- [x] Quotation handling for both single (`'`) and double (`"`) quotes
- [x] Piping between commands using `|`
- [x] Expansion for `$?` to show exit status of the last executed command
      ...

## Installation

- Clone the repository to your local machine:

```bash
git clone https://github.com/552020/minishell.git
```

- Navigate to the relevant directory `cd minishell`
- Compile the project with `make`
- Run minishell: `./minishell`

## Usage

Once `Minishell` is started, you can interact with it like any traditional Unix shell. Enter commands, use redirections (`<`, `>`, `>>`), and leverage pipes (`|`). `Minishell` also supports specific built-in commands like `echo`, `cd`, and `pwd`. To exit, type `exit` or press `CTRL-D`.

## Constraints

### Allowed Functions

For the development of `Minishell`, we were restricted to using a specific set of functions. The permitted external functions are:

- I/O Functions: `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `write`, `read`, `open`, `close`
- Memory Management: `malloc`, `free`
- Process Management: `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `exit`, `execve`
- Signal Handling: `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`
- File and Directory Handling: `access`, `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `dup`, `dup2`, `pipe`, `opendir`, `readdir`, `closedir`
- Error Handling: `strerror`, `perror`
- Terminal Control: `isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

In addition to the aforementioned functions, we were permitted to use our self-coded `libft` library. This library, a previous project from the School 42 curriculum, contains a set of 40 functions that serve as replacements for common C library functions like `strcpy`, `calloc`, and so on.

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

- _well known_: printf, malloc, free, write, open, read, close, exit
- _known_: access, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sidaddset, kill, execve, dup, dup2, pipe, strerror, perror
- _new_: readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_display, add_history, getcwd, chdir, stat, lstat, fstat, unlink, opendir, readdir, closedir, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, thetflag, tgetnum, tgetstr, tgoto, tupts

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
