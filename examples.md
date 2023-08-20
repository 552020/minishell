# EXAMPLES and EDGE CASES

## Examples

echo$USER

echo $USER | grep $ARGS $OTHER_ARGS

echo "hello world" | grep $SOME_ARGS "hello world" > output.txt

# Edge and special cases

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
