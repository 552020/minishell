# EXAMPLES and EDGE CASES

## Examples

echo$USER

echo $USER | grep $ARGS $OTHER_ARGS

echo "hello world" | grep $SOME_ARGS "hello world" > output.txt

# Edge and special cases

## Spaces between command and arguments

TODO: handle this

```bash
echo"Hello"
```

The output is command not found. Exactly `bash: echoHello: command not found`. NB: it's not giving bach echo"Hello" command not found but echoHello command not found. The double quotes got stripped.

```bash
bash-3.2$ ec"ho" "Hello"
Hello
bash-3.2$ ec"ho""Hello"
bash: echoHello: command not found
bash-3.2$ echo'Hello'
bash: echoHello: command not found
bash-3.2$ ec'ho''Hello'
bash: echoHello: command not found
bash-3.2$ ec'ho' 'Hello'
Hello
bash-3.2$
```

## HEREDOC edge cases

_Quoted delmiteer_ The delimiter can be quoted. If it's quoted, then the content of the heredoc will be treated literally, and no parameter expansion, command substitution, or arithmetic expansion will occur. In the following example, $HOME and `date` will be treated as literal strings and not expanded.

```bash
cat << "EOF"
$HOME
`date`
EOF
```

---

_Unquoted Delimiter_ If the delimiter is unquoted, then parameter expansion, command substitution, and arithmetic expansion will occur in the heredoc. In this case, $HOME will be expanded to the home directory, and `date` will be replaced with the current date.

```bash
cat << EOF
$HOME
`date`
EOF
```

_Variable as Delimiter_ The delimiter can be expressed as a variable.

```bash
DELIM=EOF
cat << $DELIM
This is a heredoc.
$DELIM
```

_Escaping in Deliiter_ The delimiter can contain escaped characters, but the ending delimiter must match it exactly.

```bash
cat << "EO\F"
Content here...
EO\F
```

_Nested Heredocs_ It's possible, though rare and potentially confusing, to nest heredocs.

```bash
cat << OUTER
Start of outer heredoc.
$(cat << INNER
This is the inner heredoc.
INNER
)
End of outer heredoc.
OUTER


```
