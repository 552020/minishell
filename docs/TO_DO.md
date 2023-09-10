# TO DOs

- access vs stat functions: which to use for what?

- built-in commands vs normal commands: which is the best place to differenciate between them? Lexer, Parse or Execution? The mutual friend has pretty strong opinion about where the cut should be made, she says after the parser, in the execution step: https://chat.openai.com/share/84489c0c-e049-4469-b15f-dd81a018fe35

- heredoc: heredocs are passed to the stdin are not strings passed as argument to a command, multiple heredocs with different delimiters are possible
- ttyname and ttyslot. They seem to be unnecessary for the project, check if someone else is using them.

- `echo"Hello"` - see examples and edge cases.
- `mkdir "mydir""ectory"` vs `mkdir "mydir" "ectory"`. The first command will create only a directory `mydirectory`, the second command will create two directories `mydir` and `ectory`

- Eventually implement edge cases for heredoc delimiter. It could be in quotes or it could be a variable.
