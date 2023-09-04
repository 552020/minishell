# TO DOs

- Fix heredoc: the delimiter string is not saved correctly
- ttyname and ttyslot. They seem to be unnecessary for the project, check if someone else is using them.

- `echo"Hello"` - see examples and edge cases.
- `mkdir "mydir""ectory"` vs `mkdir "mydir" "ectory"`. The first command will create only a directory `mydirectory`, the second command will create two directories `mydir` and `ectory`

- Eventually implement edge cases for heredoc delimiter. It could be in quotes or it could be a variable.
