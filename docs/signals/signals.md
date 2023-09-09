# Signals

## Subject

- Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

- In interactive mode:
  ctrl-C displays a new prompt on a new line.
  ctrl-D exits the shell.
  ctrl-\ does nothing.

## Evaluation Sheet

### Signals

- ctrl-C in an empty prompt should display a new line with a new prompt.
- ctrl-\ in an empty prompt should not do anything.
- ctrl-D in an empty prompt should quit minishell --> RELAUNCH!

- ctrl-C in a prompt after you wrote some stuff should display a new line with a new prompt.
- The buffer should be clean too. Press "Enter" to make sure nothing from the previous line is executed.
- ctrl-D in a prompt after you wrote some stuff should not do anything.
- ctrl-\ in a prompt after you wrote some stuff should not do anything.

- Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
- Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.
- Try ctrl-D after running a blocking command like cat without arguments or grep “something“.

- Repeat multiple times using different commands.

### Go Crazy and history

- Type a command line, then use ctrl-C and press "Enter". The buffer should be clean and there should be nothing left to execute.
