# Minishell

A 'simple' Unix shell in C for the School 42 curriculum.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation & Usage](#installation--usage)
- [Constratints](#constraints)
- [Architecture](#architecture)
- [Contributors](#contributors)

## Introduction

`Minishell` is a simple Unix shell developed in C as part of the School 42 curriculum. It is a collaborative project developed by [Batu](https://github.com/BatuSengezer) and [Stefano](https://github.com/552020). The program is being developed with certain constraints: - the strictly adherence to the so called `Normi`, which imposes certain (at times questionable) limitations such as a maximum function length of 25 lines, the prohibition of for-loops, and [more](add_internal_link/to_the_norm), a restricted set of allowed functions.

## Features

- [x] Display a prompt when waiting for a new command.
- [x] Working history.
- [x] Input redirection (`<`) and output redirection (`>` & `>>`)
- [x] Handling of environment variables (like `$PATH`)
- [x] Signal management for `ctrl-C`, `ctrl-D` and `ctrl-\`
- [x] Built-in commands support (`echo`, `cd`, `pwd`, `export`, `unset`, `exit`, `pwd`)
- [x] Quotation handling for both single (`'`) and double (`"`) quotes
- [x] Piping between commands using `|`
- [x] Expansion for `$?` to show exit status of the last executed command
- [x] ...

## Installation & Usage

- Clone the repository to your local machine: `git clone https://github.com/552020/minishell.git`
- Navigate to the relevant directory `cd minishell`
- Compile the project with `make`
- Run minishell: `./minishell`

Once `Minishell` is started, you can interact with it like any traditional Unix shell. Enter commands, use redirections (`<`, `>`, `>>`), and pipe commands.

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

### The Norm

The most notable restrictions of the norm are:

- the 42 header should be present in every file
- functions must not exceed 25 lines
- global variables are generally not allowed
- the use of ceratin keywords as `for`, `do...while`, `switch`, `case`, and `goto`.
- Comments are not permitted within functions​​.
- Blank spaces within a function are not allowed, except between variable declarations and the rest of the code​​.
- All variables must be declared and set at the top of the function​​.

The entire set of rules and guidelines established by the Norm can be found in the document [`en.norm.pdf`](./docs/en.norm.pdf) within the repository.

## Architecture
