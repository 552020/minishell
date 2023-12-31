# build_pattern

This function will build a "pattern" from a string containing a wildcard. Example: from a string like "cat file\_.txt",
it will build a pattern like `file*.txt`.
The arguments are:

    - asterisk: a pointer to the first asterisk in the string. Example: `file*.txt`
    -> `*`

    - input_start: a pointer to the beginning of the string. Example: `cat file*.txt`
    -> `c`

    - pattern_ptr: a pointer to a t_pattern variable. This variable will be filled with the pattern. We use this struct not only in this function but also in the `get_matching_entries` function.

The return value is void but actually the point of the function is to fill the t_pattern variable,
the pattern field, which is the whole pattern like "file*.txt". The suffix,
prefix and midfixes fields are also filled. The prefix in this example is "file" and the suffix is ".txt". The midfixes are the strings between the asterisks. In this example,
there are no midfixes. Examples of midfixes in a pattern like "file*hello*world*.txt*",
"hello" and "world". The reason to distinguish between them is that we can check prefix and suffix with ft_strncmp,
but we need to check midfixes with ft_strstr.
*/

## Notes to build pattern

/\*

- First while: bring pattern->start to the beginning of the pattern
- Second while; bring pattern->start to the beginning of the pattern
- Clean the pattern from double asterisks
  pattern->pattern = reduce_consecutive_char(pattern_raw, '\*');
- Reset pattern->start and pattern->end to the beginning and to the end of the pattern
  pattern->start = pattern->pattern;
- Build the prefix
  asterisk = ft_strchr(pattern->pattern, '\*')
- Build the midfixes
  asterisk*reader = (char *)asterisk + 0;
  \_/

## Notes to reduce_consecutive_char

/\*
General idea:
Reduce consecutive characters to one
Partiuculare notses:

- We use this pointer so that 'str' is always pointing to the beginning of the str. Same for this one.
  while (_src_ptr)
  _/

```c
 // We do this so input points always to the beginning of the string
//*input = vars->ret;
// We do this so that we can repeat the loop from the beginning in wildcard_expansion
// vars->str = *input;

void	wildcard_expansion_build_expansion(t_wildcard *vars, char **input,
		t_data *data)
{
	build_pattern(vars->str, *input, &vars->pattern, data);
	vars->matched_files = get_matching_entries(&vars->pattern, data, vars);
	vars->before = ft_substr(*input, 0, vars->pattern.input_pattern_start
			- *input);
	vars->after = ft_substr(vars->pattern.input_pattern_end, 0,
			ft_strlen(vars->pattern.input_pattern_end));
	vars->tmp = ft_strjoin(vars->before, vars->matched_files);
	vars->ret = ft_strjoin(vars->tmp, vars->after);
	free_t_wildcard(vars);
	vars->tmp = vars->ret;
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	*input = vars->ret;
	vars->str = *input;
}

```

## Why are the array entries and matching defined as `t_entry **`?

The question is that we are going to rewrite matching, on every check with memcpy. We will create a new copy of the array holding the pointers to the t_entry structs, copy the matching structs in the new array and delete the previous array. For that we need a pointer 'behind' them.

```c
typedef struct s_entry
{
	char		*entry;
	char		*idx;
}				t_entry;

typedef struct s_entries
{
	t_entry		**entries;
	t_entry		**matching;
	int			count;
}				t_entries;
```
